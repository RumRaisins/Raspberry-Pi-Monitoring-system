/*************************************************************************
	> File Name: client.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed 24 Jan 2018 08:31:25 PM DST
 ************************************************************************/

#include"client.h"




int Client::start(){

	printf("start port %d\n" , port);

	int sockfd = socket_create(port);

	int sockaccept = socket_accept(sockfd);

	int state = 0;
	while(1){
		recv(sockaccept , &state ,sizeof(state) ,0);
		switch(state){
			case 100 :
				send_file((this->readpath + "cpu.log").c_str() , state);
				printf("cpu send end\n");
				break;
			case 101 :
				send_file((this->readpath + "disk.log").c_str() , state);
				printf("disk send end\n");
				break;
			case 102 :
				send_file((this->readpath + "mem.log").c_str() , state);
				printf("mem send end\n");
				break;
			case 103 :
				send_file((this->readpath + "pro.log").c_str() , state);
				printf("pro send end\n");
				break;
			case 104 :
				send_file((this->readpath + "sysinfo.log").c_str() , state);
				printf("sysinfo send end\n");
				break;
			case 105 :
				send_file((this->readpath + "users.log").c_str() , state);
				printf("user.log send end\n");
				break;
			default:
				continue;
		}
		printf("state = %d\n" , state);
		state++;
		send(sockaccept, &state ,sizeof(state),0); 
		state = 0;
	}

	close(sockaccept);
	close(sockfd);
	return 0;
}

int Client::send_file(const char* filename , int state){

	

	int sockfd = socket_create(8732);
	int sockaccept = socket_accept(sockfd);

	int ack = 0;
	recv(sockaccept , &ack , sizeof(ack) , 0);
	if(ack = 1){
		printf("start to send!\n");
	}
	FILE *fd = nullptr;
	char data[MAX_CHAR_SIZE];
	size_t num_read;
	fd = fopen(filename, "r");
	if(!fd){
		perror("File open errpr\n");
		state += 300;
		send(sockaccept, &state ,sizeof(state),0); 
	}else{
		std::cout <<"File open success\n";
		state += 100;
		send(sockaccept, &state ,sizeof(state),0); 
		while(1){
			num_read = fread(data , 1 , MAX_CHAR_SIZE, fd);
			if(num_read == 0)
				break; 
			printf("%s\n" , data);
			if(num_read < 0)
				perror("error in fread()\n");
			if(send(sockaccept , data , num_read , 0) < 0)
				perror("error in sending file.\n");
		}
	} 	

	fclose(fd);
	close(sockaccept);
	close(sockfd);
	return 0;
}
