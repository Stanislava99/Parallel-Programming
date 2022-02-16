#include<stdio.h>
#include<stdlib.h>
#include<mpi.h> 

int main(int argc, char *argv[]) {
	int p, id;
	int i, mysize, allsize;
	int *allarr, *myarr; // nizi za prakjanje i primanje 
	int *counts, *displ; // counts, displacements
	
	MPI_Init(&argc, &argv); 
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	
	mysize = id + 1;
	myarr = (int *) malloc(mysize * sizeof(int));

	for (i = 0; i < mysize; i++) {
		myarr[i] = id + 1;
	}

	if (id == 0) {
		counts = (int *) malloc(p * sizeof(int)); // kolku elementi prakja sekoj proces
		displ = (int *) malloc(p * sizeof(int)); // od koja pozicija vo nizata allray treba da se zapishe pratenoto
	}
	
	// MPI_Gather(shto podatok se prakja, kolku podatoci se prakjaat, tip na podatok, kade kje bidat primeni podatocite, kolku se prima od sekoj proces, tipot na podatok, root, COMM);
	MPI_Gather(myarr, 1, MPI_INT, counts, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (id == 0) {
		displ[0] = 0;
		for (i = 1; i < p; i++) {
			displ[i] = displ[i-1] + counts[i-1];
		}

		allsize = 0;
		for(i = 0; i < p; i++) {
			allsize = allsize + counts[i]; // finalna dolzhina
		}
		allarr = (int *) malloc(allsize * sizeof(int));

	}

	// MPI_Gatherv(shto se prakja, kolkava e dolzhinata na pratenoto, tipot na podatoci, kade e priemot, kolku vrednosti da se ochekuvaat od koj proces, kade da se zapishat primenite vrednosti za toj proces, tipot na podatok, root, COMM);
	MPI_Gatherv(myarr, mysize, MPI_INT, allarr, counts, displ, MPI_INT, 0, MPI_COMM_WORLD);

	if (id == 0) {
		for (i = 0; i < allsize; i++) {
			printf("%d, ", allarr[i]);
		}
		printf("\n");
	}

	MPI_Finalize();

	return 0;
}
