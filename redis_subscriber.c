#include "redis_subscriber.h"
#include <hiredis/adapters/libevent.h>

void sub_callback(redisAsyncContext* ctx, void* reply, void* privdata)
{
	redisReply* rp = (redisReply*)reply;
	if(!rp){
		printf("Invalid parameter.\n");
		return;
	}

	if(rp->type == REDIS_REPLY_ARRAY && rp->elements == 3){
		if(strcmp(rp->element[0]->str, "subscribe") != 0){
			printf("Received [%s] channel %s: %s\n", (char*)privdata, rp->element[1]->str, rp->element[2]->str);
		}
	}
}

void connect_callback(const redisAsyncContext* ctx, int status)
{
	if(status != REDIS_OK){
		printf("Error: %s\n", ctx->errstr);
		return;
	}
	printf("Connected...\n");
}

void disconnect_callback(const redisAsyncContext* ctx, int status)
{
	if(status != REDIS_OK){
		printf("Error: %s\n", ctx->errstr);
		return;
	}
	printf("Disconnected...\n");
}

int subscribe(const char* host, int port, const char* channel)
{
	if(!host || !channel){
		printf("Invalid parameters.\n");
		return -1;
	}

	signal(SIGPIPE, SIG_IGN);
	struct event_base* e_base = event_base_new();

	redisAsyncContext* ctx = redisAsyncConnect(host, port);
	if(ctx->err){
		printf("Error: %s\n", ctx->errstr);
		return -1;
	}

	redisLibeventAttach(ctx, e_base);

	redisAsyncSetConnectCallback(ctx, connect_callback);

	redisAsyncSetDisconnectCallback(ctx, disconnect_callback);

	redisAsyncCommand(ctx, sub_callback, (char*)"ZHOU", "SUBSCRIBE %s", channel);

	event_base_dispatch(e_base);
	// event_base_eloop()

	return 0;
}

