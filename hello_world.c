#include <mpi.h>
#include <stdio.h>

int main (int argc, char** argv) {
    MPI_Init(NULL,NULL);

    int my_rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    printf("Hello! My rank is %d\n",my_rank);

    MPI_Finalize();
}