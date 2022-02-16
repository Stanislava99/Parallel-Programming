#include <stdio.h>
#include <math.h>
#include <stdlib.h>


int main (int argc, char **argv){
    int id,size;

    MPI_Init( &argc , &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    int array_size, as;
    int i;
    int *array;

    if (id == 0){ 
        printf ("Enter the size of the array : ");
        scanf("%d", &array_size);

        as = (int *) malloc(array_size * sizeof(int));

        for (i=0;i<array_size;i++){
            array[i] = i+1;
            printf("%d ",array[i]);
        }

        
    }
}