#include "redis_publisher.h"
#include <pthread.h>

int init_semevent(PSemevent pse)
{
	if(!pse)return -1;
	int ret = 0;

	pse->e_base = event_base_new();
	if(!pse->e_base){
		printf("Failed to create redis event.\n");
		return -1;
	}

	memset(&pse->e_sem, 0x00, sizeof(pse->e_sem));
	ret = sem_init(&pse->e_sem, 0, 0);
	if(ret != 0){
		printf("Failed to initialize sem.\n");
		release_semevent(pse);
		return -1;
	}
	return 0;
}

int release_semevent(PSemevent pse)
{
	if(!pse)return -1;

	pse->e_base = NULL; //// Just this ???
	sem_destroy(&pse->e_sem);
	return 0;
}

void* event_thread(void* data)
{
	if(!data){
		printf("Data is NULL.\n");
		return NULL;
	}

	printf("event_thread...\n");

	PSemevent pse = (PSemevent)data;
	sem_wait(&pse->e_sem);
	event_base_dispatch(pse->e_base);

	return NULL;
}

void command_callback(redisAsyncContext* ctx, void* reply, void* privdata)
{
	printf("Publish command callback...\n");
	*((int*)privdata) = 1;
}

void connect_callback(const redisAsyncContext* ctx, int status)
{
	if(status != REDIS_OK){
		printf("Failed to connect to redis server.\n");
	}else{
		printf("Successed to connect to redis.\n");
	}
}

void disconnect_callback(const redisAsyncContext* ctx, int status)
{
	if(status != REDIS_OK){
		printf("Failed to disconnect to redis server.\n");
	}else{
		printf("Successed to disconnect to redis server.\n");
	}
}

int publish(const char* channel, const char* str)
{
	if(!channel || !str)return -1;
	int ret = 0;
	pthread_t tid;
	Semevent se;
	int command_end_flag = 0;

	ret = init_semevent(&se);
	if(ret != 0){
		printf("Failed to init semevent.\n");
		return -1;
	}

	redisAsyncContext* ctx = redisAsyncConnect("127.0.0.1", 6379);
	if(!ctx){
		printf("Failed to connect to redis-server.\n");
		release_semevent(&se);
		return -1;
	}

	if(ctx->err){
		printf("Error: %s\n", ctx->errstr);
		release_semevent(&se);
		return -1;
	}

	redisLibeventAttach(ctx, se.e_base);

	ret = pthread_create(&tid, 0, &event_thread, (void*)&se);
	if(ret != 0){
		printf("Failed to create thread.\n");
		release_semevent(&se);
		return -1;
	}

	redisAsyncSetConnectCallback(ctx, connect_callback);
	redisAsyncSetDisconnectCallback(ctx, disconnect_callback);

	sem_post(&se.e_sem);

	ret = redisAsyncCommand(ctx, command_callback, (void*)&command_end_flag, "PUBLISH %s %s", channel, str);
	if(ret == REDIS_ERR){
		printf("Failed to publish...\n");
	}

	while(command_end_flag != 1){
		usleep(10);
	}

	// disconnect
	if(ctx){
		redisAsyncDisconnect(ctx);
		//redisAsyncFree(ctx);
		ctx = NULL;
	}

	// release
	release_semevent(&se);

	return 0;
}
