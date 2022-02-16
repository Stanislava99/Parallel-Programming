#include<stdio.h>
#include<stdlib.h>
#include<mpi.h> 

int main(int argc, char *argv[]) {
	int p, id;
	int *sray, *rray;
	int *sdisp, *scounts, *rdisp, *rcounts;
	int ssize, rsize;
	int i;
	
	MPI_Init(&argc, &argv); 
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	// Alltoall(); - static
	// Alltoallv(); - dynamic

	// sekoj proces treba da znae kolku podatoci prakja kade, kolku podatoci prima kade
	scounts = (int *) malloc(p * sizeof(int));
	rcounts = (int *) malloc(p * sizeof(int));
	sdisp = (int *) malloc(p * sizeof(int));
	rdisp = (int *) malloc(p * sizeof(int));

	/*
	0: scounts = [1, 2, 3, 4]
	1: scounts = [1, 2, 3, 4] 
	preku all to all se dobiva:
	0: rcounts = [1, 1, 1, 1] -> kolku elementi prima nulti proces od sekoj drug
	1: rcounts = [2, 2, 2, 2] -> 			prviot
	// sekoj proces kje prakja onolku vrednosti kolku shto e id-to + 1 na procesot kon koj prakja
	// ona shto se prakja e id;

	// presmetka na displ
	0: sdispl = 0, 1, 3, 6
	1: sdispl = 0, 1, 3, 6

	0: rdispl = 0, 1, 2, 3
	1: rdispl = 0, 2, 4, 6

	0, niza = 0, 1, 2, 3, 
	1, niza = 0, 0, 1, 1, 2, 2, 3, 3, 
	3, niza = 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 
	2, niza = 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 
	*/

	for (i = 0; i < p; i++) {
		scounts[i] = i + 1;
		// printf("%d\n", scounts[i]); // [1, 2, 3, 4]
	}

	MPI_Alltoall(scounts, 1, MPI_INT, rcounts, 1, MPI_INT, MPI_COMM_WORLD); 

	sdisp[0] = 0;
	for (i = 1; i < p; i++) {
		sdisp[i] = scounts[i-1] + sdisp[i-1];
	}

	rdisp[0] = 0;
	for (i = 1; i < p; i++) {
		rdisp[i] = rcounts[i-1] + rdisp[i-1];
	}

	for (i = 0; i < p; i++) {
		printf("%d)\n", i);
		printf("%d %d\n", scounts[i], rcounts[i]);
		printf("%d %d\n", sdisp[i], rdisp[i]);
	}

	ssize = 0;
	rsize = 0;

	for(i = 0; i < p; i++) {
		ssize += scounts[i];
		rsize += rcounts[i];
	}
	
	sray = (int *) malloc(ssize * sizeof(int));
	rray = (int *) malloc(rsize * sizeof(int));

	for (i = 0; i < ssize; i++) {
		sray[i] = id; // ona shto se prakja
	}

	MPI_Alltoallv(sray, scounts, sdisp, MPI_INT, rray, rcounts, rdisp, MPI_INT, MPI_COMM_WORLD);

	printf("%d, niza = ", id);
	for (i = 0; i < rsize; i++) {
		printf("%d, ", rray[i]);
	}
	printf("\n");

	MPI_Finalize();

	return 0;
}










