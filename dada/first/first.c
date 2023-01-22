#include <mpi.h>
#include <stdio.h>
#define ROOT 0
#define MSG_TAG 100

int main(int argc, char **argv)
{
	int tid,size;
	MPI_Status status;

	MPI_Init(&argc, &argv); //Musi być w każdym programie na początku

	printf("Checking!\n");
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Comm_rank( MPI_COMM_WORLD, &tid );
	printf("My id is %d from %d\n",tid, size);

	int msg;

	msg = 0;

	if (tid == ROOT) {
		MPI_Send(&msg, 1, MPI_INT, tid + 1, MSG_TAG, MPI_COMM_WORLD );
		printf(" Wyslalem %d do %d\n", msg, tid );

		MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf(" Otrzymalem %d  od %d\n", msg, status.MPI_SOURCE);
	} else {
		MPI_Recv(&msg, 1, MPI_INT, tid - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf(" Otrzymalem %d  od %d\n", msg, status.MPI_SOURCE);

		if (tid == size - 1) {
			MPI_Send(&msg, 1, MPI_INT, ROOT, MSG_TAG, MPI_COMM_WORLD );
			printf(" Wyslalem %d do %d\n", msg, tid );
		} else {
			MPI_Send(&msg, 1, MPI_INT, tid + 1, MSG_TAG, MPI_COMM_WORLD );
			printf(" Wyslalem %d do %d\n", msg, tid );
		}
	}

	MPI_Finalize(); // Musi być w każdym programie na końcu
}
