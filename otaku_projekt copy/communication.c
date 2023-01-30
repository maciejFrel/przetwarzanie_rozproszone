#include "otaku.h"
#include "communication.h"

void *startRecieving(void *ptr) {
    packet_data packet;
    MPI_Status status;

    while (1) {
        debug("waiting for message");
        MPI_Recv(&packet, 1, MPI_PACKET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        debug("recieved a message");
    }
}