#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

// multiply two matrices by elements

int main(int argc, char **argv)
{
    int i,j,l;
    int n = 3;
    int a[n][n], b[n][n], c[n][n];
    int count, overflow, offset, rows = 0;
    int id, p;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(id == 0)
    {
        l=1;
        for(i=0;i<n;i++)
        {
            for(j=0;j<n;j++)
            {
                a[i][j] = l;
                b[i][j] = l;
                l++;
            }
        }

        count = n/p;
        overflow = n%p;
        offset = (id <overflow)? count+1: count;

        for(i=1;i<p;i++)
        {
            rows = (i < overflow)? count+1: count;
            MPI_Send(&offset, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&a[offset][0], rows*n, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&b[offset][0], rows*n, MPI_INT, i, 0, MPI_COMM_WORLD);
            offset += rows;
        }
    }

    if(id != 0)
    {
        MPI_Recv(&offset, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, rows*n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, rows*n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    if(id == 0)
    {
        rows = (id <overflow)? count+1: count;
    }

    for(i=0;i<rows;i++)
    {
        for(j=0;j<n;j++)
        {
            c[i][j] = a[i][j]*b[i][j];
            printf(" %d  ", c[i][j]);
        }
    }

    if(id != 0)
    {
        MPI_Send(&offset,1,MPI_INT,0,1,MPI_COMM_WORLD);
        MPI_Send(&rows,1,MPI_INT,0,1,MPI_COMM_WORLD);
        MPI_Send(&c,rows*n,MPI_INT,0,1,MPI_COMM_WORLD);
    }
    else 
    {
        for(i=1;i<p;i++)
        {
            MPI_Recv(&offset,1,MPI_INT,i,1,MPI_COMM_WORLD,&status);
            MPI_Recv(&rows,1,MPI_INT,i,1,MPI_COMM_WORLD,&status);
            MPI_Recv(&c[offset][0],rows*n,MPI_INT,i,1,MPI_COMM_WORLD,&status);
        }
        printf("----------------\n");
        for(i=0;i<n;i++)
        {
            for(j=0;j<n;j++)
            {
                printf("%d ",c[i][j]);
            }
            printf("\n");
        }
        printf("----------------\n");

    }

    MPI_Finalize();
    return 0;
}