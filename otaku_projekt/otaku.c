#include "otaku.h"
#include "main_thread.h"
#include "communication_thread.h"

int rank, size;

state_t state = Out;
int m = 0;
int p = 0;
int x = 0;

pthread_t communicationThread, mainThread;
pthread_mutex_t stateMutex = PTHREAD_MUTEX_INITIALIZER;

void finalize()
{
    pthread_mutex_destroy(&stateMutex);
    pthread_join(communicationThread, NULL);
    MPI_Type_free(&MPI_PACKET_T);
    MPI_Finalize();
}

// void check_thread_support(int provided)
// {
//     printf("THREAD SUPPORT: chcemy %d. Co otrzymamy?\n", provided);
//     switch (provided)
//     {
//     case MPI_THREAD_SINGLE:
//         printf("Brak wsparcia dla wątków, kończę\n");
//         /* Nie ma co, trzeba wychodzić */
//         fprintf(stderr, "Brak wystarczającego wsparcia dla wątków - wychodzę!\n");
//         MPI_Finalize();
//         exit(-1);
//         break;
//     case MPI_THREAD_FUNNELED:
//         printf("tylko te wątki, ktore wykonaly mpi_init_thread mogą wykonać wołania do biblioteki mpi\n");
//         break;
//     case MPI_THREAD_SERIALIZED:
//         /* Potrzebne zamki wokół wywołań biblioteki MPI */
//         printf("tylko jeden watek naraz może wykonać wołania do biblioteki MPI\n");
//         break;
//     case MPI_THREAD_MULTIPLE:
//         printf("Pełne wsparcie dla wątków\n"); /* tego chcemy. Wszystkie inne powodują problemy */
//         break;
//     default:
//         printf("Nikt nic nie wie\n");
//     }
// }

int main(int argc, char **argv)
{
    MPI_Status status;
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    // check_thread_support(provided);
    srand(rank);
    initializePacketType();
    packet_t packet;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    pthread_create(&communicationThread, NULL, startCommunicationThread, 0);

    mainLoop();

    finalize();
    return 0;
}
