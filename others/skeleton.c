#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int size,rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    
    if (rank==0) {
        
    } else {
    }
    MPI_Finalize();
}
