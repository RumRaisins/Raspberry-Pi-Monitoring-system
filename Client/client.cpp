/*************************************************************************
	> File Name: client.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed 24 Jan 2018 08:31:25 PM DST
 ************************************************************************/

#include"client.h"



int Client::sign = 0;


void *Client::updataShell(void *arg){
	FILE *fstream = nullptr;
	char buff[1024];
	string dir = "../shell/";
	while(1){
		if(nullptr == (fstream = popen("ls ../shell" ,"r"))){
			throw runtime_error("No shell in ../shell\n");
		}
		Client::lock();
		while(nullptr != fgets(buff , sizeof(buff), fstream)){
			log("bash %s" , buff);
			system((dir + buff).c_str());
			memset(buff , 0 , sizeof(buff));
		}
		Client::unlock();
		sleep(5);
	}
	pclose(fstream);
	return nullptr;
}




int Client::start(){

	pthread_t thread_shellup;
	pthread_create(&thread_shellup , NULL , Client::updataShell , NULL);


	printf("start port %d\n" , port);

	int sockfd = socket_create(port);

	int state = 0;

	bool flag = 1;
	bool first_lock = 1;
	while(flag){
		printf("Read for commond!\n");
		int sockaccept = socket_accept(sockfd);
		while(flag){
			if(recv(sockaccept , &state ,sizeof(state) ,0) == 0) {
				close(sockaccept);
				first_lock = 1;
				break;
			}
			if(first_lock){
				Client::lock();
				first_lock = 0;
			}
			switch(state){
					case 100:
					case 101:
					case 102:
					case 103:
					case 104:
					case 105:
						// 发送申请的文件
						send_control(state , sockaccept);
						break;
					case 911:
						//退出Client 
						printf("shut down!\n");
						flag = 0;
					break;
			}	
		}
		Client::unlock();
	}

	close(sockfd);
	return 0;
}





int Client::send_control(int state , int control_port){
	switch(state){
		case 100 :
			send_file((this->readpath + "cpu.log").c_str() , state , control_port);
			log("cpu send end\n");
			break;
		case 101 :
			send_file((this->readpath + "disk.log").c_str() , state , control_port);
			log("disk send end\n");
			break;
		case 102 :
			send_file((this->readpath + "mem.log").c_str() , state , control_port);
			log("mem send end\n");
			break;
		case 103 :
			send_file((this->readpath + "pro.log").c_str() , state , control_port);
			log("pro send end\n");
			break;
		case 104 :
			send_file((this->readpath + "sysinfo.log").c_str() , state , control_port);
			log("sysinfo send end\n");
			break;
		case 105 :
			send_file((this->readpath + "users.log").c_str() , state , control_port);
			log("user.log send end\n");
			break;
		default:
			break;
	}
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
		log("start to send!\n");
	}else{
		fclose(fd);
		close(sockaccept);
		close(sockfd);
		return -1;
	}
	
	char data[MAX_CHAR_SIZE];
	size_t num_read;
	log("File open success\n");

	while(1){
		num_read = fread(data , 1 , MAX_CHAR_SIZE, fd);
		if(num_read == 0)
			break; 

		log("%s" , data);

		if(num_read < 0){
			perror("error in fread()\n");
			break;
		}
		if(send(sockaccept , data , num_read , 0) < 0){
			perror("error in sending file.\n");
			break;
		}
		memset(data , 0 , sizeof(data));
	}	

	state += 300;
	send(control_port, &state ,sizeof(state),0); 

	fclose(fd);
	close(sockaccept);
	close(sockfd);
	return 0;
}
