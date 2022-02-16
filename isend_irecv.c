#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

// MPI_Isend/MPI_Irecv - non-blocking functions
// 

int main(int argc, char *argv[]) {
	int p, id;
	int source, destination, tag, count, buffer;
	MPI_Status status;
	MPI_Request request;
	
	MPI_Init(&argc, &argv); 
	MPI_Comm_size(MPI_COMM_WORLD, &p); // sekoj proces za sebe
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	destination = 1; source = 0; tag = 1234; count = 1;
	request = MPI_REQUEST_NULL;

	if (id == source) {
		buffer = 5678;
		MPI_Isend(&buffer, count, MPI_INT, destination, tag, MPI_COMM_WORLD, &request);
	}

	if (id == destination) {
		MPI_Irecv(&buffer, count, MPI_INT, source, tag, MPI_COMM_WORLD, &request);
	}

	MPI_Wait(&request, &status);

	if (id == destination) {
		printf("%d\n", buffer);
	}


	MPI_Finalize();

	return 0;
}
