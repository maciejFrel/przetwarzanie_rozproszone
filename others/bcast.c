#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	int rank, size;
	
	char bufor[100]={0};

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		strncpy( bufor, "alamakota", 10);
		printf("Wysylam\n");
	} else {
	}
	MPI_Bcast( bufor, 100, MPI_CHAR, 0, MPI_COMM_WORLD);
	if (rank !=0) {
		printf("Dostalem: %s\n", bufor);
	}
	MPI_Finalize();
}
