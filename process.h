#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SET_MAX_MEMORY(N) ((MEMORY == 0) ? MEMORY = N : -1)

#ifndef _PRCSS_H
#define _PRCSS_H

static size_t MEMORY;

typedef struct prcss{
	size_t mmry;
	size_t base;
	char* name;
}prcss;


void print_prcss(void* item){
	if (item == NULL) return;
	prcss temp = *((prcss*) item);
	printf("(%s, %ld, %ld)\n", temp.name, temp.mmry, temp.base);
}

void prcss_set_base(void* prev, void* crrnt){
	if (prev == NULL || crrnt == NULL) return;
	prcss* _one = (prcss*) prev;
	prcss* _two = (prcss*) crrnt;
	_two->base = _one->base + _one->mmry;
}

int comp_prcss(void* prcss1, void* prcss2){
	if (prcss1 == NULL || prcss2 == NULL) return 1;
	prcss* _one = (prcss*) prcss1;
	prcss* _two = (prcss*) prcss2;
	return strcmp(_one->name, _two->name);
}

int comp_prcssID(void* prcss1, void* _ID){
	if (prcss1 == NULL || _ID == NULL) return 1;
	prcss* temp = (prcss*) prcss1;
	char* id = (char*) _ID;
	return strcmp(temp->name, id);
}

#endif
