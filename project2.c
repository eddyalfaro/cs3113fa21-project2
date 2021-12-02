#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <errno.h>

#include "script.h"
#include "allo.h"

#define _BS_ "BESTFIT"
#define _FF_ "FIRSTFIT"
#define _NF_ "NEXTFIT"
#define _WF_ "WORSTFIT"

#define _false (0x00)
#define _true (~_false)

#define buff_sz (50)
#define TOTAL_COMMANDS (5) 

static char*  SCRIPTS[] = {"REQUEST", "RELEASE", "LIST AVAILABLE", "LIST ASSIGNED", "FIND"};

const char* mem_allo_algo;
const size_t* mem_sz;
const char* scriptFile;
node* (*ALLO_ALGO) (node**, prcss*);

char buffer[buff_sz];

void errExit(int errNum){
	fprintf(stderr, "Error: %s\n", strerror(errNum));
	exit(errNum);
}

char* buff_flush(){
	size_t sz = strlen(buffer) + 1;
	if (sz == 1) return NULL;

	char* _temp = malloc((sz)*sizeof(char));
	for (int i = 0; i < sz; i++) _temp[i] = buffer[i];

	return _temp;
}

void getMemSz(){
	size_t* _temp = malloc(1*sizeof(size_t));
	if (scanf("%lu", _temp) == 0) {
		exit(EXIT_FAILURE);
	}
	mem_sz = _temp;
}

void printImpl(){
	fprintf(stderr, "Implementation Error\n");
	fprintf(stdout, "\n---Implementation Format----\n");
	fprintf(stdout, "./project2 <All-Algo> <N> <ScriptFile>\n");
	fprintf(stdout, "\t<All-Algo>\n\t\tBESTFIT\n\t\tFIRSTFIT\n\t\tNEXTFIT\n\t\tWORSTFIT\n\n");
	fprintf(stdout, "\t<N>\n\t\tAny integer between 2^4 and 2^30\n\n");
	fprintf(stdout, "\t<ScriptFile>\n\t\tScript file name with the allocation commands\n\n");
	exit(EXIT_FAILURE);
}
void select_algo(char* command){
	if (strcmp(command, _FF_) == 0) {
		ALLO_ALGO = firstFit;
	}
}

int main(int argc, char** argv){

	if (argc == 1){
		scanf("%s", buffer); 
		mem_allo_algo = (buff_flush());
		
		getMemSz();
	
		scanf("%s", buffer);
		scriptFile = buff_flush();
	}else if (argc == 4){

		mem_allo_algo = (argv[1]);

		scriptFile = (argv[3]);
		
		char* endptr;
		long _temp = strtol(argv[2], &endptr, 10);
		if (strcmp(endptr, argv[2]) == 0) printImpl();
		if (errno == ERANGE || errno == EINVAL) errExit(errno);
		mem_sz = (const size_t*) &_temp;
	}else printImpl();	

	SET_MAX_MEMORY(*mem_sz);
	ASSIGN_COMMANDS(SCRIPTS, TOTAL_COMMANDS); 

	select_algo((char*) mem_allo_algo);	
	node* cmd_queue = get_command_queue((char*) scriptFile, getPrcss);
	
	node* command_node = NULL;
	node* mem_sim = NULL;
	node* checker = NULL;	

	script* command = NULL;
	prcss* _prcss = NULL;
	do{
		command_node = dequeue(&cmd_queue);

		if (command_node != NULL) command = (script*) command_node->data;
		else break;		
		
		//print_command_prcss(command);
		
		if (strcmp(command->cmd, SCRIPTS[0]) == 0){
			checker = ALLO_ALGO(&mem_sim, command->object);
			updt_lastAdded(mem_sim, checker);
			_prcss = (prcss*) command->object;

			if (checker == NULL) {
				printf("FAIL REQUEST ");
				printf("%s %lu\n", _prcss->name, _prcss->mmry); 
				//print_prcss(command->object);
				delete_prcss((prcss*) command->object);
			}else {
				printf("ALLOCATED ");
				printf("%s %lu\n", _prcss->name, _prcss->base);
				//print_prcss(command->object);
			}
		}/*else if (strcmp(command->cmd, SCRIPTS[1]) == 0){
			checker = release(mem_sim, command->object);

			if (checker == NULL){
				_prcss = (prcss*) command->object;
				printf("FAIL RELEASE %s\n", _prcss->name);
				//print_prcss(command->object);
				//delete_prcss((prcss*) command->object);
			}else {
				printf("FREE ");
				_prcss = (prcss*) checker->data;
				printf("%s %lu %lu\n", _prcss->name, _prcss->mmry, _prcss->base);
				//print_prcss(checker->data);
				
				//if removed the same as the last added it is kept
				//otherwise it is deleted from memory
				//if (comp_prcss(checker->data, last_add->data) != 0)
				//	delete_node(checker, delete_prcss);
			}	
		}else if(strcmp(command->cmd, SCRIPTS[2]) == 0){
			listAvailable(mem_sim);
		}else if(strcmp(command->cmd, SCRIPTS[3]) == 0){
			printList(&mem_sim, print_prcss);
		}else if(strcmp(command->cmd, SCRIPTS[4]) == 0){
			
		}*/else {
			print_command_prcss(command);
		}
		//printf("**************\n");
		//delete_node(command_node, NULL);
		//delete_script(command);

	}while (cmd_queue != NULL);		

	/*
	if (argc == 1){
		free((char*) mem_allo_algo);
		free((size_t*) mem_sz);
		free((char*) scriptFile);
	}
	*/
	return 0;
}
