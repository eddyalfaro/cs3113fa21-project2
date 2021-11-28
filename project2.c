#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <errno.h>

#include "node.h"
#include "process.h"
#include "script.h"

#define _BS_ "BESTFIT"
#define _FF_ "FIRSTFIT"
#define _NF_ "NEXTFIT"
#define _WF_ "WORSTFIT"

#define _false (0x00)
#define _true (~_false)

#define buff_sz (50)

const char* mem_allo_algo;
const size_t* mem_sz;
const char* scriptFile;

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
	scanf("%lu", _temp);
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

	

	if (argc == 1){
		free((char*) mem_allo_algo);
		free((size_t*) mem_sz);
		free((char*) scriptFile);
	}

	exit(EXIT_SUCCESS);
}
