/*************************************************************************
	> File Name: master.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed 24 Jan 2018 08:31:00 PM DST
 ************************************************************************/
#include "master.h"


int Master::start(){
	string client_name = "client";
	
	int client_num = -1;
	auto client_num_s = this->parameters.find("client_num");
	if(client_num_s != this->parameters.end()){
		client_num = atoi(client_num_s->second.c_str());
	}
	while(1){
		bool flag = 1;
		int num = 0;
		while(flag){
			auto client = this->parameters.find(client_name + (char )('1' + num));
			if(client == parameters.end()){
				printf("Num of Client is \n");
				if(client_num > 0){
					printf("Num of Client is %d in config\n" , num);
				}		
			}
			int pid = fork();
			if(pid == 0){
				receive_from_client(client);
				flag = 0;
				return 0;
			}else{
				log("pid = %d solve %s\n" , pid , client->second.c_str() );
			}
			if(num++ > client_num){
				flag = 0;
			}
		}
		sleep(5);
	}
	return 0;
}


int Master::receive_from_client(map<string , string>::iterator client){
	//printf("connect user port = %d, ip = %s\n" , this->port , client->second.c_str());
	int sock_con = socket_connect(this->port , client->second.c_str());
	if(sock_con < 0) {
		printf("%s connect failed\n" , client->first.c_str());
		return -1;
	}
	char p[10];
	for(int state = 100 ; state < 106 ; state++){
		sed_seponse(sock_con , state);
		state_to_char(state , p);
		if(recv_response(sock_con , state + 100) != 0){
			debugERROR(p , " NOT FOUND");
			continue;
		}
		
		writefile( client->second, this->writepath +client->first , p);	
		if(recv_response(sock_con , state+300) != 0){
			debugERROR(p , "RECEIVE ERROR");
		}
	}	


	close(sock_con);
	return 0;
}
int Master::writefile(const string& client_ip,const string& file_path , const char *name ){

	
	log("start port %d , ready for data\n" , 8851);

	int sock_con = socket_connect(8851 , client_ip.c_str());


   if(access(file_path.c_str(),0)==-1)//access函数是查看文件是不是存在
    {
    	log("Don't have log file dir , make dir\n");
        if (mkdir(file_path.c_str(),0777))//如果不存在就用mkdir函数来创建
        {
            log("creat file bag failed!!!");
        }
    }
    int ack = 1;
    send(sock_con, &ack,sizeof(ack),0); 


	char data[MAX_CHAR_SIZE];
	int size;

	FILE *fd = fopen((file_path + "/" +name).c_str(),"wt+");
	while((size = recv(sock_con , data , 1024 , 0)) > 0 ){

		log("%s" ,data);

		fwrite(data , 1 , size , fd);
		memset(data , 0 , sizeof(data));
	}	

	if(size < 0)
		perror("write file error\n");
	if(size == 0){
		log("recervice success!\n");
	}

	fclose(fd);
	close(sock_con);

	return 0;
}
/*
string  Master::whosendme(int sockaccept){
	sockaddr_in peer;
	socklen_t peer_t;

	bzero(&peer , sizeof(peer));
	getpeername(sockaccept , (sockaddr *)&peer , &peer_t);

	char buff_peer[64]  = {'\0'};

    if (inet_ntop(AF_INET, (void *)&peer.sin_addr, buff_peer, 63))
    {
        std::cout << "\npeer  ip: "      << buff_peer
                  << "\tpeer port: "  << ntohs(peer.sin_port);
    }
    
    std::string target = ntohs(peer.sin_port);

    auto find_item = std::find_if(this->parameters.begin(), this->parameters.end(),
        [target](const std::map<string, string>::value_type item)
    {
        return item.second == target;
    });

    if (find_item!= this->parameters.end())
    {
        return (*find_item).first;
    }else{
    	printf("No %s is blone client" , target.c_str());
    	return nullptr;
    }
    
}
*/


	

