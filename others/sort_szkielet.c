#include <mpi.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#define SORT 1
#define END 2
#define TABSIZE 10000

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int size,rank;
    int tablica[TABSIZE]={0};
    int sorted[TABSIZE]={-1};
    int max=-1;
    int tmp=-1;
    int i;
    int end=0;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc<2) {
	    MPI_Finalize();
	    printf("Za malo argumentow\n");
	    exit(0);
    }
    if (rank == 0) {
        printf("Otwieram plik\n");
	FILE *f;int i;
	f = fopen(argv[1],"r");
	for (i=0;i<TABSIZE;i++) fscanf(f, "%d", &(tablica[i]));
	for (i=0;i<TABSIZE;i++) printf("%d ", (tablica[i]));

        max=tablica[0];
        printf("\n------------\n");
	printf("%d: Wczytuje %d, wysylam\n", rank, max);
        for (i=1;i<TABSIZE;i++) {
               // MPI_Send(&(tablica[i]), 1, MPI_INT, rank+1, SORT, MPI_COMM_WORLD);
        }
 //       MPI_Send( &max, 1, MPI_INT, rank+1, END, MPI_COMM_WORLD);
///
	MPI_Recv(&sorted, TABSIZE-(size-1), MPI_INT, size-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for (i=0;i<TABSIZE-(size-1);i++) {
            printf("%d ",sorted[i]);
        }
        printf("\n");
    } else if (rank==size-1) { //LISC
    //
    } else {
    //
    }
   
    MPI_Finalize();
}
