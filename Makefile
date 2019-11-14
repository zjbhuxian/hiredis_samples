CC=gcc
CFLAGS=-Wall -O -g
DFLAGS=-lhiredis -levent -lpthread
OBJS_pub=pub.o redis_publisher.o
OBJS_sub=sub.o redis_subscriber.o
TARGET_pub=pub
TARGET_sub=sub

all:$(TARGET_sub) $(TARGET_pub)

$(TARGET_pub):$(OBJS_pub)
	$(CC) $^ $(DFLAGS) -o $@

$(TARGET_sub):$(OBJS_sub)
	$(CC) $^ $(DFLAGS) -o $@

$(OBJS_pub):%.o:%.c
	$(CC) $^ $(CFLAGS) -c

$(OBJS_sub):%.o:%.c
	$(CC) $^ $(CFLAGS) -c

clean:
	rm -rf *.o

clean_all:clean
	rm -rf $(TARGET_pub)  $(TARGET_sub)
