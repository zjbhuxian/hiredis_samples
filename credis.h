#ifndef __CREDIS_H__
#define __CREDIS_H__
#include <hiredis/hiredis.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int num;
	char** strArray;
}Sets, *PSets;

void init_sets(PSets ps);
void free_sets(PSets ps);

int store_handle(const char* host, int port, const char* key, const char* mstr);
int fetch_handle(const char* host, int port, const char* key, PSets ps);
#endif
