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
* MPI_ Status status - returns the MPI status

Process numbers will not be in ascending order, because the processes execute independently without waiting for eachother. Each time you execute the program, you can get different results.

#### MPI Communication Routes

* MPI_Send(void *data_to_send, int send_count, MPI_Datatype send_type, 
      int destination_ID, int tag, MPI_Comm comm) - sends a message to another process <br/>
      <br/>
    - data_to_send: variable of a C type that corresponds to the send_type supplied below <br/>
    - send_count: number of data elements to be sent (nonnegative int) <br/>
    - send_type: datatype of the data to be sent (one of the MPI datatype handles) <br/>
    - destination_ID: process ID of destination (int) <br/>
    - tag: message tag (int) <br/>
    - comm: communicator (handle) <br/>

*  MPI_Recv(void *received_data, int receive_count, MPI_Datatype receive_type, 
      int sender_ID, int tag, MPI_Comm comm, MPI_Status *status) - receives a message from another process <br/>
      <br/>
    - received_data: variable of a C type that corresponds to the receive_type supplied below  <br/>
    - receive_count: number of data elements expected (int) <br/>
    - receive_type: datatype of the data to be received (one of the MPI datatype handles) <br/>
    - sender_ID: process ID of the sending process (int)   <br/>
    - tag: message tag (int) <br/>
    - comm: communicator (handle) <br/>
    - status: status struct (MPI_Status) <br/>
 <br/> <br/>
Some of datatypes recognized by MPI are:
MPI_INT	- int
MPI_SHORT	- short
MPI_LONG	- long
MPI_FLOAT	- float
MPI_DOUBLE	- double
MPI_CHAR	- char
MPI_BYTE	- unsigned char


#### Collective data movement

* MPI_Bcast(void *data_to_be_sent, int send_count, MPI_Datatype send_type, 
      int broadcasting_process_ID, MPI_Comm comm); - broadcasts data to all other processes
* MPI_Scatter(void *send_data, int send_count, MPI_Datatype send_type, 
      void *receive_data, int receive_count, MPI_Datatype receive_type, 
      int sending_process_ID, MPI_Comm comm); - Break the data into portions and send those portions separately to other processes
* MPI_Gather - gathers the data from participating processes into a single structure
* MPI_Allgather - gathers data from different processes into a single structure that is then sent to all participants 
