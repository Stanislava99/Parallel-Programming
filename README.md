# Parallel-Programming
Parallel Programming in C with MPI and OpenMP

### MPI

This is a short introduction to the Message Passing Interface (MPI). This instruction should deliver enough informations to write simple parallel C programs using MPI.

#### Hello World
```c
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
  MPI_Init(&argc, &argv);
  printf("Hello wordl");
  fflush(stdout);
  MPI_Finalize();
  }
```
* MPI_Init(&argc, &argv) - Initialization of MPI 
* MPI_Finalize() - End of MPI
* fflush(stdout) - Recommended to use after printf()

To compile and run the program use 
$ mpicc hello.c -o hello 
$ mpirun -np 4 hello - np 4 tells the program to set number of processes to 4 (you can use any number you want)

#### Separate Processes 
```c
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
  int my_id, num_procs;
  
  MPI_Init(&argc, &argv);
  
  MPI_Comm_size (MPI_COMM_WORDL, &num_procs);
  MPI_Comm_rank (MPI_COMM_WORDL, &my_id);
  
  printf("I'm process %d out of %d processes",my_id,size);
  fflush(stdout);
  
  MPI_Finalize();
  }
```
* MPI_Comm_size (MPI_COMM_WORDL, &num_procs) - returns the number of processes
* MPI_Comm_rank (MPI_COMM_WORDL, &my_id) - returns the id of the current process

Process numbers will not be in ascending order, because the processes execute independently without waiting for eachother. Each time you execute the program, you can get different results.

#### MPI Communication Routes

* MPI_Send(void *data_to_send, int send_count, MPI_Datatype send_type, 
      int destination_ID, int tag, MPI_Comm comm) - sends a message to another process
      data_to_send: variable of a C type that corresponds to the send_type supplied below
    ** send_count: number of data elements to be sent (nonnegative int)
    **send_type: datatype of the data to be sent (one of the MPI datatype handles)
    **destination_ID: process ID of destination (int)
    **tag: message tag (int)
    **comm: communicator (handle)
