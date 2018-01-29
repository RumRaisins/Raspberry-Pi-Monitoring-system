/*************************************************************************
	> File Name: master.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed 24 Jan 2018 08:31:00 PM DST
 ************************************************************************/
#include "master.h"


int Master::start(){
	string client_name = "client";
	char base = '1';
	int num = 0;
	int client_num = -1;
	auto client_num_s = this->parameters.find("client_num");
	if(client_num_s != this->parameters.end()){
		client_num = atoi(client_num_s->second.c_str());
	}
	while(1){
		auto client = this->parameters.find(client_name + (char )(base + num));
		if(client == parameters.end()){
			std::cout << "Num of Client is "<< num << std::endl;
			if(client_num > 0){
				std::cout<< "Num of Client is " << num << " in config" << std::endl;
			}
			break;
		}
		receive_from_client(client);
		num++;
	}
	return 0;
}


int Master::receive_from_client(map<string , string>::iterator client){
	printf("connect user port = %d, ip = %s\n" , this->port , client->second.c_str());
	int sock_con = socket_connect(this->port , client->second.c_str());

	if(sock_con < 0) {
		printf("%s connect failed\n" , client->first.c_str());
		return -1;
	}
	char p[10];
	for(int state = 100 ; state < 106 ; state++){
		sed_seponse(sock_con , state);
		if(recv_response(sock_con , state+100) != 0){
			debugError(state + 300);
			break;
		}
		state_to_char(state , p);
		writefile( client->second, this->writepath + client->first , p);	
	}	


	close(sock_con);
	return 0;
}
int Master::writefile(const string& client_ip,const string& file_path , const char *name ){

	
	printf("start port %d\n , ready for data" , 8732);

	int sock_con = socket_connect(8732 , client_ip.c_str());


   if(access(file_path.c_str(),0)==-1)//access函数是查看文件是不是存在
    {
    	std::cout << "Don't have log file dir , make dir\n";
        if (mkdir(file_path.c_str(),0777))//如果不存在就用mkdir函数来创建
        {
            printf("creat file bag failed!!!");
        }
    }
    int state = 1;
    send(sock_con, &state ,sizeof(state),0); 


	char data[MAX_CHAR_SIZE];
	int size;

	FILE *fd = fopen((file_path + name).c_str(),"wt+");
	while((size = recv(sock_con , data , 1024 , 0)) > 0 ){

		std::cout << data << std::endl;
		fwrite(data , 1 , size , fd);

		//fwrite("\n" , 1 , sizeof("\n") , fd);


		memset(data , 0 , sizeof(data));
	}	

	if(size < 0)
		perror("write file error\n");
	if(size == 0){
		printf("recervice success!");
	}
	fclose(fd);

	close(sock_con);

	return 0;
}



	

