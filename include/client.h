/*************************************************************************
	> File Name: client.h
	> Author: 
	> Mail: 
	> Created Time: Wed 24 Jan 2018 08:29:44 PM DST
 ************************************************************************/

#ifndef _CLIENT_H
#define _CLIENT_H

#include <pthread.h>
#include <unistd.h>

#include "common.h"





void* updataShell(void *arg);




class Client : public Common{
public:
	Client(string path) : Common(path.c_str()) {init();}
	Client(const char *path) : Common(path) {init();}

	static void *updataShell(void *arg);

	int start();
	int send_control(int state , int control_port);
	int send_file(const char* filename , int state , int control_port);

private:
	
	int port;
	int Master_data_port;
	string Master_ip;
	string readpath;

	//1被占用  0未被占用
	static int sign;

	inline static void lock(){
		while(__sync_lock_test_and_set(&sign,1)){}
	}

	inline static void unlock(){
		__sync_lock_release(&sign);
	}


	inline void init(){
		auto temp = this->parameters.find("port");

		if(temp == parameters.end()){
			printf("config hasn't port , use normal port 8731");
			port = 8731;
		}else{
			port = atoi(temp->second.c_str());
			this->parameters.erase("port");
		}


		temp = this->parameters.find("readpath");
		if(temp == parameters.end()){
			printf("config hasn't readpath , use normal in ./");
			this->readpath = "./";
		}else{
			this->readpath = temp->second;
			this->parameters.erase("readpath");		
		}

		temp = this->parameters.find("Master");
		if(temp == parameters.end()){
			throw runtime_error("config hasn't Master , Can't run ./");
		}else{
			this->Master_ip = temp->second;
			this->parameters.erase("Master");		
		}

		temp = this->parameters.find("Master_data_port");
		if(temp == parameters.end()){
			throw runtime_error("config hasn't Master , Can't run ./");
		}else{
			this->Master_data_port = atoi(temp->second.c_str());
			this->parameters.erase("Master_data_port");		
		}
	}
};



#endif
