#include<stdio.h>
#include<stdlib.h>
#include<mpi.h> 

int main(int argc, char *argv[]) {
	int p, id;
	int i, j, n, l, br, t;
	int *nizaN, *nizaL, *brResh, *pozicii;
	
	MPI_Init(&argc, &argv); 
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	if (id == 0) {
		t = scanf("%d", &n);
		nizaN = (int *) malloc(n * sizeof(int));
		for (i = 0; i < n; i++) { // [1, 2, 3, || 5, 1, 4, || 3, 1, 2, || 3, 8, 9] 
			t = scanf("%d", &nizaN[i]);
		}

		t = scanf("%d", &l);
		nizaL = (int *) malloc(l * sizeof(int));
		for (i = 0; i < l; i++) { // [1, 2, 3]
			t = scanf("%d", &nizaL[i]);
		}
	}


	// Sekoj proces pravi proverka na indeksite koi mu se dodeleni, proverkata ja pravi se dodeka ne najde podniza ili dodeka ne najde pravilo za prekin na podniza
	// MPI_Bcast(koj podatok se prakja, dimenii za podatokot, tip na podatokot, root, COMM);
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&l, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (id != 0) {
		nizaN = (int *) malloc(n * sizeof(int));
		nizaL = (int *) malloc(l * sizeof(int));
	}
	pozicii = (int *) malloc(n * sizeof(int)); 
	brResh = (int *) malloc(n * sizeof(int)); 

	MPI_Bcast(nizaN, n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(nizaL, l, MPI_INT, 0, MPI_COMM_WORLD);

	br = 0;

	for (i = id*(n/p); i < (id == p-1 ? n-l+1 : id*(n/p) + (n/p)); i++) { // 0: [0 - 2], 1: [3 - 5], ...
		int najdena = 1;

		for (j = 0; j < l; j++) {
			if (nizaN[i+j] != nizaL[j]) {
				najdena = 0;
				break;
			}
		}

		if (najdena) {
			pozicii[br++] = i;
		}
	}

	MPI_Gather(&br, 1, MPI_INT, brResh, 1, MPI_INT, 0, MPI_COMM_WORLD);



	if (id == 0) {
		for (j = 1; j < p; j++) {
			MPI_Status status;
			MPI_Recv(pozicii+br, brResh[j], MPI_INT, j, j, MPI_COMM_WORLD, &status);
			br = br + brResh[j];
			for (i = br - brResh[j]; i < br; i++) {
				printf("%d) %d\n", j, pozicii[i]);
			}
		}
		for (i = 0; i < br; i++) {
			printf("%d\n", pozicii[i]);
		}
	} else {
		MPI_Send(pozicii, br, MPI_INT, 0, id, MPI_COMM_WORLD); // tag
	}
	
	MPI_Finalize();

	return 0;
}


