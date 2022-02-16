#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

//multiply two matrices 

int main(int argc, char **argv)
{
    int id,p;
    int m1=3, m2=3, m3=3;
    int a[m1][m2], b[m2][m3], c[m1][m3];
    int i,j,k, l;
    int count, overflow, offset, rows = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(id == 0)
    {
        l = 1;
        printf("----------------\n");
        for(i=0;i<m1;i++)
        {
            for(j=0;j<m2;j++)
            {
                a[i][j] =l;
                l++;
                printf(" %d ", a[i][j]);
            }
            printf("\n");
        }
        printf("----------------\n");

        printf("----------------\n");
        l = 1;
        for(i=0;i<m2;i++)
        {
            for(j=0;j<m3;j++)
            {
                b[i][j] =l;
                l++;
                printf(" %d ", b[i][j]);
            }
            printf("\n");
        }
        printf("----------------\n");

        count = m1/p;
        overflow = m1%p;
        offset = (id < overflow) ? count + 1 : count;

        for(i=1;i<p;i++)
        {
            rows = (i < overflow) ? count + 1 : count;
            MPI_Send(&offset,1,MPI_INT,i,0,MPI_COMM_WORLD);
            MPI_Send(&rows,1,MPI_INT,i,0,MPI_COMM_WORLD);
            MPI_Send(&a[offset][0],rows*m2,MPI_INT,i,0,MPI_COMM_WORLD);
            MPI_Send(&b,m2*m3,MPI_INT,i,0,MPI_COMM_WORLD);
            offset += rows;
        }
    }

    if(id != 0)
    {
        MPI_Recv(&offset,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
        MPI_Recv(&rows,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
        MPI_Recv(&a,rows*m2,MPI_INT,0,0,MPI_COMM_WORLD,&status);
        MPI_Recv(&b,m2*m3,MPI_INT,0,0,MPI_COMM_WORLD,&status);
    }

    if(id == 0)
    {
        rows = (id<overflow) ? count + 1 : count;
    }

    for(i=0;i<rows;i++)
    {
        for(j=0;j<m3;j++)
        {
            c[i][j] = 0;
            for(k=0;k<m2;k++)
            {
                c[i][j] += a[i][k]*b[k][j];
            }
        }
    }

    if(id !=0)
    {
        MPI_Send(&offset,1,MPI_INT,0,1,MPI_COMM_WORLD);
        MPI_Send(&rows,1,MPI_INT,0,1,MPI_COMM_WORLD);
        MPI_Send(&c,rows*m3,MPI_INT,0,1,MPI_COMM_WORLD);
    }
    else
    {
        for(i=1;i<p;i++)
        {
            MPI_Recv(&offset,1,MPI_INT,i,1,MPI_COMM_WORLD,&status);
            MPI_Recv(&rows,1,MPI_INT,i,1,MPI_COMM_WORLD,&status);
            MPI_Recv(&c[offset][0],rows*m3,MPI_INT,i,1,MPI_COMM_WORLD,&status);
        }
        printf("----------------\n");
        for(i=0;i<m1;i++)
        {
            for(j=0;j<m3;j++)
            {
                printf(" %d ", c[i][j]);
            }
            printf("\n");
        }
        printf("----------------\n");
    }
    MPI_Finalize();

    return 0;
}