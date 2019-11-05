#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <hiredis/hiredis.h>
#include "credis.h"


//void doTest()
//{
//	redisContext* c = redisConnect("127.0.0.1", 6379);
//	if(c->err){
//		redisFree(c);
//		printf("Failed to connect to redisServer.\n");
//		return;
//	}
//
//	printf("Successed to connect to redisServer.\n");
//
//	const char* command1 = "set stest1 value1";
//	redisReply* r = (redisReply*)redisCommand(c, command1);
//
//	if(NULL == r){
//		printf("Failed to execute command1.\n");
//		redisFree(c);
//		return;
//	}
//
//	if(!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK") == 0)){
//		printf("Failed to execute command[%s]\n", command1);
//		freeReplyObject(r);
//		redisFree(c);
//		return;
//	}
//	freeReplyObject(r);
//	printf("Successed to execute command[%s]\n", command1);
//
//	const char* command2 = "strlen stest1";
//	r = (redisReply*)redisCommand(c, command2);
//	if(r->type != REDIS_REPLY_INTEGER){
//		printf("Failed to execute command[%s]\n", command2);
//		freeReplyObject(r);
//		redisFree(c);
//		return;
//	}
//
//	int length = r->integer;
//	freeReplyObject(r);
//	printf("The length of 'stest1' is %d.\n", length);
//	printf("Successed to execute command[%s]\n", command2);
//
//	const char* command3 = "get stest1";
//	r = (redisReply*)redisCommand(c, command3);
//	if(r->type != REDIS_REPLY_STRING){
//		printf("Failed to execute command[%s]\n", command3);
//		freeReplyObject(r);
//		redisFree(c);
//		return;
//	}
//	printf("The value of 'stest1' is %s\n", r->str);
//	freeReplyObject(r);
//	printf("Successed to execute command[%s]\n", command3);
//
//	const char* command4 = "get stest2";
//	r = (redisReply*)redisCommand(c, command4);
//	if(r->type != REDIS_REPLY_NIL){
//		printf("Failed to execute command[%s]\n", command4);
//		freeReplyObject(c);
//		return;
//	}
//	freeReplyObject(r);
//	printf("Successed to execute command[%s]\n", command4);
//
//	redisFree(c);
//}

int main(int argc, char** argv)
{
	Sets s;
	init_sets(&s);

	int ret = 0;
	int i = 0;

	if(argc <= 1){
		return -1;
	}
	ret = store_handle("127.0.0.1", 6379, "zhou", argv[1]);
	if(ret != 0){
		printf("Failed to store.\n");
		return -1;
	}

	ret = fetch_handle("127.0.0.1", 6379, "zhou", &s);
	if(ret != 0){
		printf("Failed to fetch.\n");
		free_sets(&s);
		return -1;
	}

	printf("s.num = [%d]\n", s.num);
	for(i = 0; i < s.num; ++i){
		printf("member[%d]: %s\n", i, s.strArray[i]);
	}

	free_sets(&s);
	return 0;
}
