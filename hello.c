#include <stdio.h>
#include <mpi.h>

int main (int argc, char **argv){

    int number_procs, my_id;

    MPI_Init(&argc, &argv);

    MPI_Comm_size( MPI_COMM_WORLD , &number_procs); 
    MPI_Comm_rank( MPI_COMM_WORLD , &my_id);

    printf ("ID : %d, NP: %d\n",my_id, number_procs);
    fflush(stdout);

    MPI_Finalize();


}