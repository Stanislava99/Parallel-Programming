#include<stdio.h>
#include<stdlib.h>
#include<mpi.h> 

int main(int argc, char *argv[]) {
	int p, id;
	int i, j, k;
	int m1 = 6, m2 = 4, m3 = 3; // m2 e zaednichko
	int count, overflow, offset, rows;	

	// 4 reda i 5 koloni: 2 procesi -> 20 -> 10 elementi po proces -> 2 redici
	// 5 reda i 5 koloni: 2 procesi -> 25 -> 1: 13, 2: 12 -> 2 za matricata -> pola elementi vo prv, pola elementi vo vtor
	// 5 reda i 5 koloni: 2 procesi -> 1: 15, 2: 10 !!!

	double a[m1][m2], b[m2][m3], c[m1][m3];
	MPI_Status status;
	
	MPI_Init(&argc, &argv); 
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	if (id == 0) {
		for(i = 0; i < m1; i++) {
			for(j = 0; j < m2; j++) {
				a[i][j] = i + j;
				// printf("a[%d][%d] = %f, ", i, j, a[i][j]);
			}
		}
		for(i = 0; i < m2; i++) {
			for(j = 0; j < m3; j++) {
				b[i][j] = i + j;
				// printf("b[%d][%d] = %f, ", i, j, b[i][j]);
			}
		}

		// da ja pratime cela b, i da pratime del od a (da se podeli rabotata megju procesi)
		// cela b ja prakjam so m2*m3
		// za a: kaj sekoj proces kolku prakjam, dali za nekoj proces postoi vishok redica (overflow), od kade vo matricata kje prakjam za sekoj proces
		count = m1 / p; // kolku minimum redici kje primi sekoj proces !!
		overflow = m1 % p; // na kolku procesi kje treba da dademe plus redica
		offset = (id < overflow) ? count + 1 : count; // SAMO ZA NULTI PROCES - ZA SEKOJ SLEDEN KJE TREBA DA SE UPDATE-ne
		printf("%d|%d|%d", count, overflow, offset);

		for (i = 1; i < p; i++) {
			rows = (i < overflow) ? count + 1 : count; // Proces so id = i kje dobie volku redici
			MPI_Send(&offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&rows, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&a[offset][0], rows * m2, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
			MPI_Send(&b, m2 * m3, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
			offset += rows;
		}
	}

	if (id > 0) {
		MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&rows, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&a, rows * m2, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&b, m2 * m3, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
	}

	if (id == 0) {
		rows = (id < overflow) ? count + 1 : count;
	}

	for (i = 0; i < m3; i++) {
		for (j = 0; j < rows; j++) {
			c[j][i] = 0.0;
			for (k = 0; k < m2; k++) {
				c[j][i] = c[j][i] + a[j][k] * b[k][i];
			}
			printf("c[%d][%d] = %f, ", j, i, c[j][i]);
		}
	}

	if (id > 0) {
		MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
		MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
		MPI_Send(&c, rows * m3, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
	} else {
		for(i = 1; i < p; i++) {
			MPI_Recv(&offset, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
			MPI_Recv(&rows, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
			MPI_Recv(&c[offset][0], rows * m3, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &status);
		}

		printf("\n************************\n");
		for (i = 0; i < m1; i++) {
			for (j = 0; j < m3; j++) {
				printf("%6.2f", c[i][j]);
			}
			printf("\n");
		}
		printf("\n************************\n");
	}

	MPI_Finalize();

	return 0;
}

