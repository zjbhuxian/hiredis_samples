#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "redis_subscriber.h"

int main(int argc, char** argv)
{
	if(argc != 2){
		printf("<prog> <channel>\n");
		return -1;
	}

	int ret = subscribe("127.0.0.1", 6379, argv[1]);
	if(ret != 0){
	}
	return 0;
}
