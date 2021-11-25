#Eddy Alfaro Cordoba

## Compiling
compiling can be done with the command "make all"

## Executing
Once the file has binary has been compiled the program can be executed as 
```
./project1 FILEPATH
```

The program has already a set of files to run as an input if you use the command 

```
make run
```

this set of files will be used and the output will be presented in the terminal.

## Bugs
bugs can be expected when any of the Ids of the processes presented in the input file is not within the range of (0, p]. This bug is due to the way the processes are currently being stored. its process ID correspond to the element number which it occupies within the array.

## Resources
> https://afteracademy.com/blog/what-is-burst-arrival-exit-response-waiting-turnaround-time-and-throughput
>
> https://pubs.opengroup.org/onlinepubs/7908799/xsh/open.html
>
> https://www.tutorialspoint.com/cprogramming/c_error_handling.htm
>
> https://linux.die.net/man/2/close
