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
	char* name; 	//Dynamically
}prcss;


void print_prcss(void* item){
	if (item == NULL) {printf("\n"); return;}
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

void* getPrcss(char* info){
	if (info == NULL) return NULL;
	prcss* temp = malloc(sizeof(prcss));
	
	char* token = strtok(info, " \t\n");
	size_t size = strlen(token);
	temp->name = malloc((size + 1)*sizeof(char));
	memcpy(temp->name, token, size + 1);
	
	token = strtok(NULL, " \t\n");
	if (token == NULL) return temp;

	temp->mmry = strtol(token, NULL, 10);
	return temp;	
}

void delete_prcss(void* target){
	prcss* item = (prcss*) target;
	if (item == NULL) return;
	if (item->name != NULL) free(item->name);
	free(item);
}

#endif
