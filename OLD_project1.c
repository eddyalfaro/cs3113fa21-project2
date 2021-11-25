#include<stdio.h>
#include<stdlib.h>

struct Process{
	int _pid;
	int _totalBurst;
	int _unvSwitch;
	int _waitTime;
	int _startToFinish;
};

struct Process* newProcess(int _pid,int _totalBurst);
int compare(struct Process* _lhs, struct Process* _rhs);
void moveUp(struct Process* _Array, int _from, int *_size);
void printProcess(struct Process* _prcss);
void join(struct Process* _array, int* _size);

int main (int argc, char** argv) {
	int P, p, N;
	char* _fileName = argv[1];
	FILE* file = fopen(_fileName, "r");

	fscanf(file, "%d", &P);
	fscanf(file, "%d %d", &p, &N);
	int volSwitch = N - 1;
	struct Process* _PSchedule = malloc((N + 1)*sizeof(_PSchedule));
	
	int _pid, _totalBurst, _priority;
	struct Process* _temp;

	for(int i = 0; i < N; i++){
		fscanf(file, "%d %d %d", &_pid, &_totalBurst, &_priority);
		_temp = newProcess(_pid, _totalBurst);
		_PSchedule[i] = *_temp;
		if (i == 0) _PSchedule[i]._waitTime = 0;
		else _PSchedule[i]._waitTime = _PSchedule[i - 1]._waitTime + _PSchedule[i - 1]._totalBurst;
	}

	printf("\n>>>>Initial Queue Switches: %d\n", N - 1);
	for(int i = 0; i < N; i++){
		printf("**%d. <", i);
		printProcess(&_PSchedule[i]);
		printf(">\n");
	}

	join(_PSchedule, &N);	

	printf("\n>>>>Removing concurrent processes\n");
	for(int i = 0; i < N; i++){
		printf("**%d. <", i);
		printProcess(&_PSchedule[i]);
		printf(">\n");
	}

	printf("\n");
	printf("\n>>>>Joining unvoluntary switched  processes\n");
	int lastWait, lastBurst;
	for(int i = 0; i < N; i++){
		_temp = &_PSchedule[i];
		printf("(%d,%d)", i, i);
		printProcess(_temp);
		printf("\n");
		lastWait = 0;
		lastBurst = 0;
		for(int j = i + 1; j < N; j++){
			if (compare(_temp, &_PSchedule[j])==0){
				_temp->_totalBurst += _PSchedule[j]._totalBurst;
				_temp->_unvSwitch++;
				lastWait = _PSchedule[j]._waitTime;
				lastBurst = _PSchedule[j]._totalBurst;
				moveUp(_PSchedule, j + 1, &N);
				printf("\t(%d,%d)", i, j);
				printProcess(&_PSchedule[j]);
				printf("\n");
			}
		}
		if (lastWait != 0) _temp->_startToFinish = (lastWait - _temp->_waitTime + lastBurst);
		else _temp->_startToFinish = _temp->_totalBurst;
	}
	
	_temp = NULL;
	printf("\n>>>>Merged queue\n");
	for(int i = 0; i < N; i++){
		printf("**%d. <", i);
		printProcess(&_PSchedule[i]);
		printf(">\n");
	}
	
	int totalUnvSwitch = 0;
	for (int i = 0 ; i < N; i++) totalUnvSwitch += _PSchedule[i]._unvSwitch;
	volSwitch -= totalUnvSwitch;
	

	printf("\nStatistics\n");
	printf("%d\n%d\n100.00\n", volSwitch, totalUnvSwitch);
	
	float _BurstTotal = 0, _TimeFrameTotal = 0, _WaitTimeTotal = 0;
	for (int i = 0; i < N; i++){
		_BurstTotal += _PSchedule[i]._totalBurst;
		_TimeFrameTotal += _PSchedule[i]._startToFinish;
		_WaitTimeTotal += _PSchedule[i]._waitTime;
	}
	
	printf("\n\nUsing time from Start to Finish\n");
	printf("TP = %.2f\nTT = %.2f\nWT = %.2f\nRT = %.2f\n", p/_TimeFrameTotal, (_TimeFrameTotal + _WaitTimeTotal)/p, _WaitTimeTotal/p, _WaitTimeTotal/p);

	printf("\n\nUsing total burst time\n");
	printf("TP = %.2f\nTT = %.2f\nWT = %.2f\nRT = %.2f\n\n\n", p/_BurstTotal, (_BurstTotal + _WaitTimeTotal)/p, _WaitTimeTotal/p, _WaitTimeTotal/p);
	
	free(_PSchedule);
        return 0;
}

void join(struct Process* _array, int* _size){
	for(int i = 0; i < *_size - 1; i++){
		while (compare(&_array[i], &_array[i + 1]) == 0 && i < *_size - 1){
			_array[i]._totalBurst += _array[i + 1]._totalBurst;
			moveUp(_array, i + 1, _size);
		}
	}
}

void moveUp(struct Process* _Array, int _from, int *_size){
	for (int i = _from; i < *_size - 1; i++) _Array[i] = _Array[i + 1];
	(*_size)--;
}

void printProcess(struct Process* _prcss){
	printf("PID = %d, Burst = %d, Wait Time = %d, UnvSwitch = %d, StarttoFinish = %d", _prcss->_pid, _prcss->_totalBurst, _prcss->_waitTime, _prcss->_unvSwitch, _prcss->_startToFinish);
}

struct Process* newProcess(int _pid, int _totalBurst){
	struct Process* _temp = malloc(sizeof(_temp));
	_temp->_pid = _pid;
	_temp->_totalBurst = _totalBurst;
	_temp->_unvSwitch = 0;
	return _temp;
}

int compare(struct Process* _lhs, struct Process* _rhs){
	return _lhs->_pid - _rhs->_pid;
}
