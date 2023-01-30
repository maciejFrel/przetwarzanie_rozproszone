#include "otaku.h"
#include "util.h"
MPI_Datatype MPI_PACKET_T;

void inicjuj_typ_pakietu()
{
    int blocklengths[NUMBER_OF_PACKET_ITEMS] = { 1, 1, 1, 1 };
    MPI_Datatype types[NUMBER_OF_PACKET_ITEMS] = { MPI_INT, MPI_INT, MPI_INT, MPI_INT };

    MPI_Aint offsets[NUMBER_OF_PACKET_ITEMS];
    offsets[0] = offsetof(packet_data, type);
    offsets[1] = offsetof(packet_data, p);
    offsets[2] = offsetof(packet_data, m);
    offsets[3] = offsetof(packet_data, x);

    MPI_Type_create_struct(NUMBER_OF_PACKET_ITEMS, blocklengths, offsets, types, &MPI_PACKET_T);

    MPI_Type_commit(&MPI_PACKET_T);
}

void sendPacket(packet_data *pkt, int destination, int tag)
{
    int freepkt = 0;
    if (pkt == 0)
    {
        pkt = malloc(sizeof(packet_data));
        freepkt = 1;
    }
    // pkt->type = rank;
    MPI_Send(pkt, 1, MPI_PACKET_T, destination, tag, MPI_COMM_WORLD);
    debug("Wysyłam %s do %d\n", tag2string(tag), destination);
    if (freepkt)
        free(pkt);
}