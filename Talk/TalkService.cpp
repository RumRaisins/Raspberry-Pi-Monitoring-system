
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
#include <iostream>

const int port = 8731;


int socket_create(int port){
	int sockfd;
	int yes = 1;
	struct sockaddr_in sock_addr;

	//创建套接字
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() error\n");
		return -1;
	}

	//设置本地套接字地址
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port); //转化为网络字节序
	sock_addr.sin_addr.s_addr = htonl(INADDR_ANY); //0.0.0.0

	//设置本地套接字
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		close(sockfd);
		perror("setsockopt() error\n");
		return -1;
	}

	//绑定本地套接字到套接字
	if (bind(sockfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0) {
		close(sockfd);
		perror("bind() error\n");
		return -1;
	}

	//将套接字设置为监听状态
	if (listen(sockfd, 20) < 0) {
		close(sockfd);
		perror("listen() error");
		return -1;
	}
	return sockfd;
}


/*
*接受套接字请求
*返回新的套接字描述符，错误返回-1
*/
int socket_accept(int sock_listen) {
	int sockfd;
	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr); 
	sockfd = accept(sock_listen, (struct sockaddr *) &client_addr, &len);
	if (sockfd < 0){
		perror("accept() error");
		return -1;
	}
	return sockfd;
}


/*
*接受套接字请求
*返回新的套接字描述符，错误返回-1
*/

int socket_connect(int port, char *host) {
	int sockfd;
	struct sockaddr_in dest_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() error");
		return -1;
	}

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	dest_addr.sin_addr.s_addr = inet_addr(host);


	if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
		perror("connect() error");
		return -1;
	}

	return sockfd;

}
void debugSorctName(int sockaccept){
	sockaddr_in local ,peer;
	socklen_t local_t , peer_t;
	bzero(&local , sizeof(local));
	bzero(&peer , sizeof(peer));

	getsockname(sockaccept , (sockaddr *)&local , &local_t);
	getpeername(sockaccept , (sockaddr *)&peer , &peer_t);

	char buff_local[64] = {'\0'}, buff_peer[64]  = {'\0'};

	if (inet_ntop(AF_INET, (void *)&local.sin_addr, buff_local, 63))
	{
    	std::cout << "local ip: "        << buff_local
                 << "\tlocatl port: "  << ntohs(local.sin_port);
    }
    if (inet_ntop(AF_INET, (void *)&peer.sin_addr, buff_peer, 63))
    {
        std::cout << "\npeer  ip: "      << buff_peer
                  << "\tpeer port: "  << ntohs(peer.sin_port);
    }
}

int main(){
	int sockfd = socket_create(port);

	int sockaccept = socket_accept(sockfd);

	char buffer[1024];
    char *resend = "cover that!\n";
    int resend_len = strlen(resend);
	while(1){
        if(sockaccept != -1) {
    		debugSorctName(sockaccept);
    		std::cout << "\nsay : ";
        }
		memset(buffer , 0 , sizeof(buffer));
        int len = recv(sockaccept , buffer , sizeof(buffer),0);
		printf("%s" , buffer);
        fflush(stdout);
	    send(sockaccept , resend , resend_len ,0);
	}
	close(sockaccept);
	close(sockfd);
	return 0;
}
