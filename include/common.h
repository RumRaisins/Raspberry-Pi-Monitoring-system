
/*
*100 CPU
*101 MEM
*102 DISK
*103 PRO
*104 SYSINFO
*105 USERINFO
*
*
*CLIENT REPLY CODE
*400 CPU FILE NOT FOUND
*401 MEN FILE NOT FOUND
*402 DISK FILE NOT FOUND
*403 PRO FILE NOT FOUND
*404 SYSINFO FILE NOT FOUND
*405 USERINFO FILE NOT FOUND
*
*
*CLIENT REPLAY CODE
*200 CPU file found and ready to transter
*201 MEM file found and ready to transter
*202 DISK file found and ready to transter
*203 PRO file found and ready to transter
*204 SYSINFO file found and ready to transter
*205 USERINFO file found and ready to transter
*
*
*
*300 CPU normal OK
*301 MEN normal OK
*302 DISK normal OK
*303 PRO normal OK
*304 SYSINFO normal OK
*305 USERINFO normal OK
*
*
*500 CPU file finished
*501 MEM file finished
*502 DISK file finished
*503 PRO file finished
*504 SYSINFO file finished
*504 USERINFO file finished
*/











#ifndef COMMON_H
#define COMMON_H

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>		
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <map>
#include <iostream>
#include<fstream>

#include <sys/stat.h>
using std::map;
using std::runtime_error;
using std::string;
using std::ifstream;


#define MAX_CHAR_SIZE 1024


#ifdef DEBUG
#define log(frm , arg...){\
	printf(frm , ##arg);\
}
#else 
#define log(frm , arg...){\
}

#endif



class Common{
public:
	Common(const char *path);

	int socket_create(int port);

	int socket_accept(int sock_listen);

	int socket_connect(int port, const char *host);

	int recv_data(int sockfd, const char* buf, int bufsize);

	int send_response(int sockfd, int rc);

	void state_to_char(int state,char* );

protected:
	map<string , string> parameters;


};



#endif