#include "otaku.h"
#include "communication.h"

int main(int argc, char **argv)
{
    int size, tid, provided;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &tid);

    // printf("%d %d %d \n", size, tid, provided);

    MPI_Finalize();

    return 0;
}
