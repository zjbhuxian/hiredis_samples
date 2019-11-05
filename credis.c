#include "credis.h"

void init_sets(PSets ps)
{
	if(!ps)return;

	ps->num = 0;
	ps->strArray = NULL;
}

void free_sets(PSets ps)
{
	if(!ps)return;

	int num = ps->num;
	int index = 0;
	char* p = NULL;
	if(num > 0){
		for(index = 0; index < num; ++index){
			p = ps->strArray[index];
			if(p){
				free(p);
				ps->strArray[index] = NULL;
			}
			ps->num -= 1;
		}
	}
}

int store_handle(const char* host, int port, const char* key, const char* mstr)
{
	if(!host || !key || !mstr){
		printf("Invalid parameters.\n");
		return -1;
	}

	redisContext* rc = redisConnect(host, port);
	if(!rc){
		printf("Failed to get connection.\n");
		return -1;
	}

	char* ptemp = "SADD %s %s";
	char	com[1024] = {0};
	sprintf(com, ptemp, key, mstr);
	printf("Com: [%s]\n", com);

	redisReply* rp = (redisReply*)redisCommand(rc, com);
	if(rp == NULL){
		printf("Failed to execute command [%s]", com);
		redisFree(rc);
		return -1;
	}

	printf("rp->type = [%d]\n", rp->type);
	if(!(rp->type == REDIS_REPLY_INTEGER)){
		printf("Failed to add to redis. error [%s]\n", rp->str);
		freeReplyObject(rp);
		redisFree(rc);
		return -1;
	}

	freeReplyObject(rp);
	redisFree(rc);
	return 0;
}

int fetch_handle(const char* host, int port, const char* key, PSets ps)
{
	if(!host || !key || !ps){
		printf("Invalid parameters.\n");
		return -1;
	}

	redisContext* rc = redisConnect(host, port);
	if(!rc){
		printf("Failed to get connection.\n");
		return -1;
	}

	char* ptemp = "SMEMBERS %s";
	char	com[1024] = {0};
	int		num = 0;
	int		i = 0;
	size_t	len = 0;
	sprintf(com, ptemp, key);
	printf("Com: [%s]\n", com);

	redisReply* rp = (redisReply*)redisCommand(rc, com);
	if(rp == NULL){
		printf("Failed to execute command [%s]\n",com);
		return -1;
	}

	if(!(rp->type == REDIS_REPLY_ARRAY)){
		printf("Failed to fetch values by the key [%s]\n", key);
		freeReplyObject(rp);
		redisFree(rc);
		return -1;
	}

	num = rp->elements;
	//ps->num = num;
	printf("Num = %d\n", num); //zhou++ for test
	ps->strArray = (char**)malloc(num * sizeof(char*));
	if(!ps->strArray){
		printf("Failed to malloc memory.\n");
		ps->num = 0;
		freeReplyObject(rp);
		redisFree(rc);
		return -1;
	}

	for(i = 0; i < num; ++i){
		len = strlen(rp->element[i]->str);
		if(len > 0){
			ps->strArray[i] = (char*)malloc(len+1);
			if(!ps->strArray[i]){
				printf("Failed to malloc memory.\n");
				free_sets(ps);
				freeReplyObject(rp);
				redisFree(rc);
				return -1;
			}
			memset(ps->strArray[i], 0x00, len+1);
			strncpy(ps->strArray[i], rp->element[i]->str, len);
			ps->strArray[i][len] = '\0';
			ps->num += 1;
		}
	}

	freeReplyObject(rp);
	redisFree(rc);
	return 0;
}

