#ifndef __REDIS_SUBSCRIBER_H__
#define __REDIS_SUBSCRIBER_H__
#include <stdio.h>
#include <stdlib.h>
#include <hiredis/adapters/libevent.h>
#include <string.h>
#include <signal.h>

typedef void (*callback)(redisAsyncContext* ctx, void* reply, void* privdata);

void sub_callback(redisAsyncContext* ctx, void* reply, void* privdata);

void connect_callback(const redisAsyncContext* ctx, int status);

void disconnect_callback(const redisAsyncContext* ctx, int status);

int subscribe(const char* host, int port, const char* channel);

#endif
