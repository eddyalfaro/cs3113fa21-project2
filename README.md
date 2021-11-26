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
> https://docs.oracle.com/cd/E77782_01/html/E77803/stdatomic-3a.html#scrolltoc
> https://docs.oracle.com/cd/E19455-01/806-5257/sync-12/index.html
> https://en.cppreference.com/w/cpp/atomic/atomic_flag
> https://en.cppreference.com/w/cpp/atomic/memory_order
> https://www.delftstack.com/howto/c/mutex-in-c/
> https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/
> https://docs.oracle.com/cd/E19683-01/806-6867/6jfpgdcnj/index.html
