#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

int processors, num_prcss, num_instr;
int swtchs, unv_swtchs, vol_swtchs;

typedef struct prcss{
	
	int prcssId;
	int burst;
	int res_time;
	int wait_time;

}prcss;

void print(prcss* a){
	printf("<ID = %d, Burst = %d, Time of Response = %d, Wait Time = %d>\n", a->prcssId, a->burst, a->res_time, a->wait_time);
}

void errExit(int errNum){

	fprintf(stderr, "Error: %s\n", strerror(errNum));
	exit(errNum);
}

void fromFile(FILE* inpt, prcss* a){
	int _temp;
	fscanf(inpt, "%d %d %d", &(a->prcssId), &(a->burst), &_temp);
}

int isUnvol(prcss a){
	return a.wait_time - a.res_time;
}

int getTATime(prcss a){
	return a.burst + a.wait_time;
}

void scan(FILE* _file, prcss* array){
	
	unv_swtchs = 0; swtchs = num_instr - 1;

	prcss* _final = malloc((num_prcss + 1)*sizeof(prcss));
	int exec_time = 0, ta_time = 0, wtime = 0, rtime = 0, concurr = 0;
	int _temp;

	for (int i = 0; i < num_instr; i++){
		
		if (_final[array[i].prcssId].prcssId == 0){
			_final[array[i].prcssId].prcssId = array[i].prcssId;
			_final[array[i].prcssId].burst = array[i].burst;
			_final[array[i].prcssId].res_time = exec_time;
			_final[array[i].prcssId].wait_time = exec_time;

			ta_time += getTATime(_final[array[i].prcssId]);
			wtime += exec_time;
			rtime += exec_time;
		}else {
			ta_time -= getTATime(_final[array[i].prcssId]);
			_temp = _final[array[i].prcssId].wait_time; 
			wtime -= _temp;			

			_final[array[i].prcssId].wait_time = exec_time - _final[array[i].prcssId].burst;
			_final[array[i].prcssId].burst += array[i].burst;

			ta_time += getTATime(_final[array[i].prcssId]);
			wtime += _final[array[i].prcssId].wait_time; 
			if (_temp == _final[array[i].prcssId].wait_time) concurr++;
			else if (isUnvol(_final[array[i].prcssId]) != 0) unv_swtchs++;
		}		

		exec_time += array[i].burst;
	}

	vol_swtchs = swtchs - unv_swtchs - concurr + 1;
	printf("%d\n%d\n100.00\n", vol_swtchs, unv_swtchs);
	printf("%.2f\n%.2f\n%.2f\n%.2f\n",
		num_prcss/((double) exec_time), ((double) ta_time)/num_prcss, ((double)wtime)/num_prcss, 
		((double)rtime)/num_prcss);

	free(_final);
}


int main(int argc, char** argv){
	
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		errExit(errno);
	
	FILE* inputFile = fdopen(fd, "r");
	if (inputFile == NULL)
		errExit(errno);

	fscanf(inputFile, "%d %d %d", &processors, &num_prcss, &num_instr);	

	prcss* prcss_stats = malloc(num_instr*sizeof(prcss));
	for (int i = 0; i < num_instr; i++) fromFile(inputFile, &(prcss_stats[i]));

	scan(inputFile, prcss_stats);
	free(prcss_stats);	

	if (fclose(inputFile) != 0)
		errExit(errno);

	return 0;
}
