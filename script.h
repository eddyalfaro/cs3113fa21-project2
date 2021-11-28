#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

#include "process.h"
#include "node.h"

#define ASSIGN_COMMANDS(array, sz) (SCRPT_CMDS = array, NUM_OF_CMDS = sz)

#define CMMNT_CHAR '#'

#ifndef _SCRPT_H
#define _SCRPT_H

static char** SCRPT_CMDS;
static size_t NUM_OF_CMDS;

static char buffer[100];

typedef struct scriptcmd{
	char* cmd;	//Static set of rules
	char* inptArg; 	//Dynamically
	void* object; 	//Dynamically
}script;

void delete_script(script* val){
	if (val->inptArg != NULL) free(val->inptArg);
	free(val);
}

void delete_scrpt_object(script* val, void (*delete_obj) (void*)){
	if (delete_obj != NULL) delete_obj(val->object);
	delete_script(val);
}

void err_Exit(int errNum){
	fprintf(stderr, "Error: %s\n", strerror(errNum));
	exit(errNum);
}

void cleanbuffer(){
	for (int i = 0; i < 100; i++) buffer[i] = '\0';
}

char* getLine(FILE* file){
	int i = 0;
	cleanbuffer();

	while(fscanf(file, "%c", &buffer[i]) > 0){ 
		if (buffer[i] == '\n') break;
		if (buffer[i] ==  CMMNT_CHAR) {
			char grbg;
			while(fscanf(file, "%c", &grbg) > 0 && grbg != '\n');
			break;
		}
		i++;
	}
	if (i == 0) return NULL;
	return buffer;
}

script* get_scrpt_data(char* command){
	if (command == NULL) return NULL;
	script* cmmd = malloc(sizeof(script));

	char* ret;
	int i = 0;
	for( ; i < NUM_OF_CMDS; i++){
		ret = strstr(command, SCRPT_CMDS[i]);
		if (ret != NULL) break;
	}cmmd->cmd = SCRPT_CMDS[i];

	int size1 = strlen(cmmd->cmd);
	int sizeTotal = strlen(command) - size1 - 1;

	if (sizeTotal == 0) return cmmd;

	cmmd->inptArg = malloc((sizeTotal + 1)*sizeof(char));
	for (int j = 0; j < sizeTotal; j++) {
		cmmd->inptArg[j] = command[j + size1];
	}cmmd->inptArg[sizeTotal] = '\0';
	
	return cmmd;	
}

node* get_command_node(char* command, void* (*objTrans) (char* dataInfo)){
	node* temp = create_node(get_scrpt_data(command));
	script* current = (script*) temp->data;
	current->object = objTrans(current->inptArg);
	return temp;
}


void print_command_prcss(void* command){
	if (command == NULL) return;
	printf("%s :\n\t", ((script*) command)->cmd);
	print_prcss(((script*) command)->object);
}

node* get_command_queue(char* fileName, void* (*objTrans) (char* dataInfo)){
	int fd = open(fileName, O_RDONLY);
	if (fd == -1)
		err_Exit(errno);
	
	FILE* inputFile = fdopen(fd, "r");
	if (inputFile == NULL)
		err_Exit(errno);
	
	node* cmmd_queue = NULL;
	node* temp = NULL;
	do{	
		if (getLine(inputFile) == NULL) continue;		

		temp = get_command_node(buffer, objTrans);
		if (cmmd_queue == NULL) cmmd_queue = temp;
		else temp = enqueue(cmmd_queue, temp);

	}while(!feof(inputFile));
	

	fclose(inputFile);
	return cmmd_queue;		
}

#endif
