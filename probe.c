#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main (int argc,char **argv){
    int my_id, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size( MPI_COMM_WORLD , &size);
    MPI_Comm_rank( MPI_COMM_WORLD , &my_id);

    int tag = 1234;
    int buffer;
    int count = 1;
    int *i,j;

    if (my_id == 0){
        //parent process - sends
        buffer = 100;
        MPI_Send( &buffer , count , MPI_INT , 1 , tag , MPI_COMM_WORLD);
    }

    if (my_id == 1){
        // child process - receives
        MPI_Probe( 0 , tag , MPI_COMM_WORLD , &status);
        MPI_Get_count( &status , MPI_INT , &count);

        i = (int *) malloc(count * sizeof (int));
        MPI_Recv( i , count , MPI_INT , 0 , tag , MPI_COMM_WORLD , &status);

        for (j=0;j<count;j++){
            printf("%d ",i[j]);
        }
        printf("\n");

    }
    MPI_Finalize();

    return 0;
}