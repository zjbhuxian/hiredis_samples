/*************************************************************************
	> File Name: main.c
	> Author: zjbhuxian
	> Mail: zjbhuxian@163.com 
	> Created Time: Thu 14 Nov 2019 10:48:22 AM CST
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include "redis_publisher.h"

void* main_event_thread(void* data)
{
	char** argv = (char**)data;
	printf("main_event_thread...\n");
	printf("data[1], data[2] = %s,%s\n", argv[1], argv[2]);

	int ret = publish(argv[1], argv[2]);
	if(ret != 0){
		printf("Failed to execute publish.\n");
	}
	usleep(100);

	return NULL;
}


int main(int argc, char** argv)
{
	pthread_t ti[10];
	int ret;

	int num = 10;
	int i = 0;
	for(i = 0; i < num; i++){
		ret = pthread_create(&ti[i], 0, &main_event_thread, argv);
		if(ret != 0){
			printf("Failed to create pthead.\n");
			return -1;
		}
	}

	for(i = 0; i < num; i++){
		pthread_join(ti[i],NULL);
	}

	return 0;
}
