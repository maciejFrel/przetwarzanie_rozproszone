#include "otaku.h"
#include "util.h"
MPI_Datatype MPI_PACKET_T;

// struct tagNames_t
// {
//     const char *name;
//     int tag;
// } tagNames[] = {{"pakiet aplikacyjny", APP_PKT}, {"finish", FINISH}};

// const char const *tag2string(int tag)
// {
//     for (int i = 0; i < sizeof(tagNames) / sizeof(struct tagNames_t); i++)
//     {
//         if (tagNames[i].tag == tag)
//             return tagNames[i].name;
//     }
//     return "<unknown>";
// }

void initializePacketType()
{
    int blocklengths[NITEMS] = {1, 1, 1};
    MPI_Datatype typy[NITEMS] = {MPI_INT, MPI_INT, MPI_INT};

    MPI_Aint offsets[NITEMS];
    offsets[0] = offsetof(packet_t, m);
    offsets[1] = offsetof(packet_t, p);
    offsets[2] = offsetof(packet_t, x);

    MPI_Type_create_struct(NITEMS, blocklengths, offsets, typy, &MPI_PACKET_T);

    MPI_Type_commit(&MPI_PACKET_T);
}

void sendPacket(packet_t *packet, int destination, int tag)
{
    int freePacket = 0;
    if (packet == 0)
    {
        packet = malloc(sizeof(packet_t));
        freePacket = 1;
    }
    // packet->type = rank;
    MPI_Send(packet, 1, MPI_PACKET_T, destination, tag, MPI_COMM_WORLD);
    // debug("Wysyłam %s do %d\n", tag2string(tag), destination);
    if (freePacket)
        free(packet);
}

void changeState(state_t newState)
{
    pthread_mutex_lock(&stateMutex);
    if (state == In)
    {
        pthread_mutex_unlock(&stateMutex);
        return;
    }
    state = newState;
    pthread_mutex_unlock(&stateMutex);
}
