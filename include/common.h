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


#define MAX_SIZE 1024
#define MASTER_PORT 8731

struct request
{	char arg[255];
	char code[7];
};

int socket_create(int port);

int socket_accept(int sock_listen);

int socket_connect(int port, char *host);

int recv_data(int sockfd, char* buf, int bufsize);

int send_response(int sockfd, int rc);



#endif