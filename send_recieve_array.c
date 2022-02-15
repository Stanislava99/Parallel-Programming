#include <stdio.h>
#include <mpi.h>

// program that will send array from one process to another

int main (int argc, char **argv){    
    int num_procs, my_id;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    int length,l;
    int *array, *a;

    if (my_id == 0) {
        // main process 
        printf("Enter array length: ");
        fflush(stdout);
        scanf("%d", &length);

        array = (int *) malloc(length*sizeof(int)); 

        for (int i=0;i<length;i++){
            array[i] = rand() % 50; // setting random number to array element  
        }

        MPI_Send(&length,1, MPI_INT, 1, 123, MPI_COMM_WORLD); // sends the length of the array
        MPI_Send(array,length,MPI_INT,1,1234,MPI_COMM_WORLD); // sends the arrau 
    } 
    else {
        // child process 
        MPI_Recv(&l,1,MPI_INT,0,123,MPI_COMM_WORDL,&status);
        a = (int *) malloc (l*sizeof(int)); // setting array length 
        MPI_Recv(a, l, MPI_INT,0,1234,MPI_COMM_WORLD,&status);

        for (int i=0;i<l;i++) {
            printf("%d, ",a[i]);
        }
    }

    MPI_Finalize();
}