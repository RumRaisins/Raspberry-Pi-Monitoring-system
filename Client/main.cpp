#include "client.h"


int main(){
	Client a("../Client/client.conf");

	printf("start\n");

	//a.start();
	a.updataShell();
	//return a.updata();
	return 0;
}