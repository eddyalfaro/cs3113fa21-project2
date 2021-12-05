# Eddy Alfaro Cordoba

## Compiling
compiling can be done with the command "make all"

## Executing
Once the binary has been compiled the program can be executed with the following command 
```
./project2 <KEYWORD> <MEMORY SIZE> <SCRIPTFILE>
```

where: 

<__KEYWORD__> is reserved for any of the allocations algorithms used within the program. The program makes used of a singled linked list to simulate allocation of memory by an OS. each node within the linked list is an allocated program within memory. The keyword is used to determined how the program will find the required free space within memory. The reserved keywords for program implementation can be any of the followings:
  
  1. FIRSTFIT. traverses the linked list from the head node checking free space before and after the first element on the list. If no free space is found in any of these places, the program traveses the list until it finds an available location in memory with enough space for the calling program. If no space is found the process is discarded and the program will continue with the next command.
  2. BESTFIT. traverses the linked list from the head node to the tail completely. This algorithm localizes the free space in memory that will leave the smallest gap posible between the allocated process and the one following it. In the case there is no free space availabe in between already alocated processes it will simply send it back to the tail of the memory. Similar to firstfit if no available memory is found, the process is discarded and the program will continue with the next command.
  3. WORSTFIT. similarly to best fit, traverses the whole list of processes alllocated within memory. It looks for an available location that will give the biggest gap within memory between the allocated process and the one it follows. As best fit does, if there is no free space in between already allocated processes, the process is send to the tail f the list.
  4. NEXTFIT. It has a similar implementation than first fit. it differs in the initial node that is used for traversing the list. It uses a bookmark, which is updated every time there is a new process added. Using this bookmark, new processes are allocated by searching for available memory within the list from the bookmark to the tail. If no available memory is found wihtin this range the program starts looking again from the head of the list.
  
<__MEMORY SIZE__> is a integer value that gives the total ammount of memory to keep track of to the program. The memory is in bytes and it has a size of 2^k, where k ranges from 4 and 30.

<__SCRIPTFILE__> is the path or relative path of the script file that has line commands for the program. the script file commands must be separated by a new line character and comments within the file must be preceded by _#_. any string of text preceded by _#_ will be ignored by the program until a new line character is used. The commands accepted within the scope of the program are the following.

1. __REQUEST <PRCSS_ID> <PRCSS_MMRY>__. It tells the program to find available memory and allocate the program with the name of <_PRCSS_ID_> that will occupy <_PRCSS_MMRY_> ammunt of memory. the first process value is a string literal while the second must be an integer number. This command will then implement the algorithm specified when the program _./project2_ was invoked. if any memory is found for the process a printout with the keyword ALLOCATED and the process parameters is received, otherwise FAIL REQUEST will be read.
2. __RELEASE <PRCSS_ID>__. It tells the program that the memory allocated by <_PRCSS_ID_> can be released and the process can be removed from memory. IF the request to release the process memory is done by the program successfully a print out with the keyword FREE will be received, otherwise FAIL RELEASE will be read.
3. __FIND <PRCSS_ID>__. It tells the program to find the process with the name <_PRCSS_ID_> within the allocated memory. if the process is not found within memory the process will printout FAULT. In the case the process is found. the properties (Name, Memory, Location) of the process are print out.
4. __LIST AVAILABLE__. Lists the available memory as _(Memory, Location)_ if there is none FULL is print out.
5. __LIST ASSIGNED__. List the allocated processes within memory as _(Name, Memory, Location)_ if there is none NONE is print out.

The program has already a default ammount of memory set to run at with a set of files to run as an input if you use any of the following commands

```bash
make ff
make nf
make wf
make bf
```
## Bugs
* Whenever the values assigned as PRCSS_MMRY are not numeric values the program will crash.
* If the assigned values of MEMORY_SIZE are greater than 2^32 - 1 there will be an overflow error
* If the command given in the script file is one that is not recognized there will be a segmentation fault
* If any extra non-numerical values are added to the commands __RELEASE__ and __FIND__ an error will happend

## Resources
1. [size_t](https://en.cppreference.com/w/c/types/size_t)
2. [strstr()](https://en.cppreference.com/w/cpp/string/byte/strtol)
3. [atol()](https://en.cppreference.com/w/c/string/byte/atoi)
4. [strstr()](https://en.cppreference.com/w/c/string/byte/strstr)
5. [Algorithms](https://www.javatpoint.com/os-partitioning-algorithms)
