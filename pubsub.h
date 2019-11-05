#include <signal.h>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>

void subCallback(redisAsyncContext* rc, void* r, void* priv);
void connectCallback(const redisAsyncContext* rc, int status);
void disconnectCallback(const redisAsyncContext* rc, int status);

