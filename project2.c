#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void errExit(int errNum){

	fprintf(stderr, "Error: %s\n", strerror(errNum));
	exit(errNum);
}

int main(int argc, char** argv){
	
	
}
