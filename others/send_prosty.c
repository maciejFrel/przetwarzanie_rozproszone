#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#define MSG_SLV 1
int main(int argc, char **argv)
{
	int rank, size, zm;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		int i; 
		MPI_Status status;
		for (i=1;i<size;i++) {
			MPI_Recv( &zm, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			printf( "Dostalem: %d \n", zm );
		}
	} else {
		zm=rank*2;
		MPI_Send( &zm, 1, MPI_INT, 0, MSG_SLV, MPI_COMM_WORLD);
	}
	printf("Koniec\n");
	MPI_Finalize();
}
