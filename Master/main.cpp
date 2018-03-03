#include "master.h"

int main(){
	int pid = fork();
	if(pid != 0) return 0;
	Master a("/home/zhu1/Project/Internet/Master/master.conf");
	a.start();

	return 0;
}