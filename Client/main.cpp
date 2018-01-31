#include "client.h"


int main(){
	Client a("../Client/client.conf");

	printf("start\n");

	a.start();


	return 0;
}