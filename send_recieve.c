#include <stdio.h>
#include <mpi.h>

// send and recieve data buffer from one to another process

int main (int argc, char **argv) {

    int num_procs, my_id;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    int source_process = 1; 
    int destination_process = 0;
    int count = 1; // number of data files that are being sent 
    int buffer;
    int tag = 1234;

    if (my_id = source_process){
        buffer = 1010;
        MPI_Send(&buffer, count, MPI_INT, destination_process, tag, MPI_COMM_WORLD);
        printf ("Process %d sent %d\n",my_id, buffer);
    }
    else {
        MPI_Recv(&buffer, count, MPI_INT, source_process, tag, MPI_COMM_WORLD, &status);
        printf("Process %d recieved %d", my_id, buffer);
    }

    MPI_Finalize();

}