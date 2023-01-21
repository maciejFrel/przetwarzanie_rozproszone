#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define TABSIZE 20
int main(int argc, char **argv)
{
	int rank, size, suma=0;
	
	int tablica[TABSIZE];
	int dest[TABSIZE];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (argc<2) {
		MPI_Finalize();
		printf("Za malo argumentow\n");
		exit(0);
	}
	if (rank == 0) {
		FILE *f;int i;
		f = fopen(argv[1],"r");
		for (i=0;i<TABSIZE;i++) fscanf(f, "%d", &(tablica[i]));
		for (i=0;i<TABSIZE;i++) printf("%d ", (tablica[i]));
	}
	MPI_Scatter(tablica, TABSIZE/size, MPI_INT, dest, (TABSIZE/size), MPI_INT, 0, MPI_COMM_WORLD);

		int i; int j=0;
		for (i=0;i<TABSIZE/size;i++){ j+=dest[i]; printf("%d ", (dest[i]));}
		printf("%d w porzadku\n", rank);
	MPI_Gather(&j, 1, MPI_INT, &tablica, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		for (i=0;i<size;i++){ printf("%d ", (tablica[i]));}
	}

	MPI_Reduce(&j, &suma, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	printf("suma na %d == %d\n", rank, suma);

	MPI_Finalize();
}
