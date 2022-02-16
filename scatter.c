#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>


// program in which every process gets a part of data

int main (int argc,char ** argv) {
    int my_id, size ;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size( MPI_COMM_WORLD , &size);
    MPI_Comm_rank( MPI_COMM_WORLD , &my_id);

    int count = 4; // number of elements in array 
    int *array, *send_array;
    int length; 

    array = (int *) malloc (count * sizeof(int));

    if (my_id == 0) {
        // parent process 
        length = count * size;
        send_array = (int *) malloc (size * sizeof(int));

        for(int i=0;i<size;i++){
            send_array[i] = i;
        }        
    }

    MPI_Scatter( send_array , count , MPI_INT , array , count , MPI_INT , 0 , MPI_COMM_WORLD);

    int total = 0, gtotal;

    for (int i=0;i<count;i++){
        total += array[i];
    }

    printf("Locally %d total is : %d \n",my_id, total);

    MPI_Reduce( &total , &gtotal , 1 , MPI_INT , MPI_SUM , 0 , MPI_COMM_WORLD);

    if (my_id == 0) {
        printf("Final total %d",gtotal);
    }

    MPI_Finalize();
}