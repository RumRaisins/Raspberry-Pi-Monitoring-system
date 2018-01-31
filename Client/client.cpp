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

	int state = 0;

	bool flag = 1;

	while(flag){
		int sockaccept = socket_accept(sockfd);
		while(flag){
			if(recv(sockaccept , &state ,sizeof(state) ,0) == 0) {
				close(sockaccept);
				break;
			}
			switch(state){
				case 100 :
					send_file((this->readpath + "cpu.log").c_str() , state , sockaccept);
					printf("cpu send end\n");
					break;
				case 101 :
					send_file((this->readpath + "disk.log").c_str() , state , sockaccept);
					printf("disk send end\n");
					break;
				case 102 :
					send_file((this->readpath + "mem.log").c_str() , state , sockaccept);
					printf("mem send end\n");
					break;
				case 103 :
					send_file((this->readpath + "pro.log").c_str() , state , sockaccept);
					printf("pro send end\n");
					break;
				case 104 :
					send_file((this->readpath + "sysinfo.log").c_str() , state , sockaccept);
					printf("sysinfo send end\n");
					break;
				case 105 :
					send_file((this->readpath + "users.log").c_str() , state , sockaccept);
					printf("user.log send end\n");
					break;
				case 911:
					printf("shut down!\n");
					flag = 0;
				default:
					continue;
			}
			state = 0;
		}
	}

	close(sockfd);
	return 0;
}

int Client::send_file(const char* filename , int state , int control_port){


	FILE *fd = nullptr;
	fd = fopen(filename, "r");

	if(!fd){
		perror("File open errpr\n");
		state += 300;
		send(control_port, &state ,sizeof(state),0); 
		return -1;
	}
	int temp = state + 100;
	send(control_port , &temp , sizeof(temp) , 0);


	int sockfd = socket_create(8851);
	int sockaccept = socket_accept(sockfd);

	int ack = 0;
	recv(sockaccept , &ack , sizeof(ack) , 0);
	if(ack == 1){
		printf("start to send!\n");
	}else{
		fclose(fd);
		close(sockaccept);
		close(sockfd);
		return -1;
	}
	
	char data[MAX_CHAR_SIZE];
	size_t num_read;
	std::cout <<"File open success\n";

	while(1){
		num_read = fread(data , 1 , MAX_CHAR_SIZE, fd);
		if(num_read == 0)
			break; 
		
		printf("%s" , data);

		if(num_read < 0)
			perror("error in fread()\n");
		if(send(sockaccept , data , num_read , 0) < 0)
			perror("error in sending file.\n");
	}	

	state += 300;
	send(control_port, &state ,sizeof(state),0); 

	fclose(fd);
	close(sockaccept);
	close(sockfd);
	return 0;
}
