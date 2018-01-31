#include "common.h"
/*
*创建监听套接字
*返回套接字描述符，错误返回-1
*/

Common::Common(const char *path){
	ifstream infile(path);
	if(!infile){
		throw runtime_error("No config");
	}
	string line ,key , value;
	while(getline(infile , line)){
		if(line.empty()) continue;
		int start_pos = 0 , end_pos = line.size() -1 , pos;
		if((pos = line.find('#')) != -1){
			if(0 == pos){
				continue;
			}
			end_pos = pos -1;
		}
		string new_line = line.substr(start_pos , start_pos + 1 - end_pos);

		if((pos = new_line.find('=')) == -1) continue;
		key = new_line.substr(0 , pos);
		value = new_line.substr(pos + 1 , end_pos + 1- (pos + 1));
		this->parameters[key] = value;
	}
}

int Common::socket_create(int port){
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

void Common::state_to_char(int state,char* result){
	switch(state){
		case 100:
			memset(result , 0 , sizeof(result));
			strcpy(result , "/cpu.log");
			break;
		case 101:
			memset(result , 0 , sizeof(result));
			strcpy(result , "/mem.log");
			break;
		case 102:
			memset(result , 0 , sizeof(result));
			strcpy(result , "/disk.log");
			break;
		case 103:
			memset(result , 0 , sizeof(result));
			strcpy(result , "/proc.log");
			break;
		case 104:
			memset(result , 0 , sizeof(result));
			strcpy(result , "/sysinfo.log");
			break;
		case 105:
			memset(result , 0 , sizeof(result));
			strcpy(result , "/users.log");
			break;
		default:
			result = nullptr;
			throw runtime_error("No this state\n");
	}
}


/*
*接受套接字请求
*返回新的套接字描述符，错误返回-1
*/
int Common::socket_accept(int sock_listen) {
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

int Common::socket_connect(int port, const char *host) {
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

