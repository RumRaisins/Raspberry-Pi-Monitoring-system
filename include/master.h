/*************************************************************************
	> File Name: master.h
	> Author: 
	> Mail: 
	> Created Time: Wed 24 Jan 2018 08:30:08 PM DST
 ************************************************************************/

#ifndef _MASTER_H
#define _MASTER_H

#include "common.h"

class Master : public Common{
public:
	Master(string path) : Common(path.c_str()) {init();}
	Master(const char *path) : Common(path) {init();}

	int start();
	int receive_from_client(map<string , string>::iterator client);
	
    int writefile(const string& client_ip,const string& file_path , const char *name );

	inline void debugERROR( const char *file , const char *error){
		printf("%s %s\n" , file , error);
	}


	inline int sed_seponse(int master_request , int req){

		if(send(master_request , &req , sizeof(req) , 0) < 0){
			perror("send error");
			return -1;
		}
		return 0;
	}

	inline int recv_response(int sockfd , int check){
		int res_recv;
		if(recv(sockfd , &res_recv , sizeof(res_recv) , 0) < 0 || check != res_recv){
			printf("recv = %d\n" , res_recv);
			return -1;
		}
		return 0;
	}

private:
	int port;
	string writepath;


	inline void init(){
		auto temp = this->parameters.find("port");

		if(temp == parameters.end()){
			printf("config hasn't port , use normal port 8731");
			port = 8731;
		}else{
			port = atoi(temp->second.c_str());
			this->parameters.erase("port");
		}


		temp = this->parameters.find("writepath");
		if(temp == parameters.end()){
			printf("config hasn't writepath , use normal in ./");
			this->writepath = "./";
		}else{
			this->writepath = temp->second;
			this->parameters.erase("writepath");		
		}
	}

};


	



#endif
