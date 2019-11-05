#include "pubsub.h"

void subCallback(redisAsyncContext *c, void *r, void *priv) {  
    redisReply *reply = r;  
    if (reply == NULL) return;  
    if ( reply->type == REDIS_REPLY_ARRAY && reply->elements == 3 ) {  
        if ( strcmp( reply->element[0]->str, "subscribe" ) != 0 ) {  
            printf( "Received[%s] channel %s: %s\n",  
                    (char*)priv,  
                    reply->element[1]->str,  
                    reply->element[2]->str );  
        }  
    }  
}  
  
void connectCallback(const redisAsyncContext *c, int status) {  
    if (status != REDIS_OK) {  
        printf("Error: %s\n", c->errstr);  
        return;  
    }  
    printf("Connected...\n");  
}  
  
void disconnectCallback(const redisAsyncContext *c, int status) {  
    if (status != REDIS_OK) {  
        printf("Error: %s\n", c->errstr);  
        return;  
    }  
    printf("Disconnected...\n");  
}  
  
