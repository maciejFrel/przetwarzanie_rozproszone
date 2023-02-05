#include "otaku.h"
#include "util.h"
MPI_Datatype MPI_PACKET_T;

int maxFromPs(packet_t *otakuData)
{
    int max = -1;
    for (int i = 0; i < size; i++)
    {
        if (max < otakuData[i].p)
        {
            max = otakuData[i].p;
        }
    }

    return max;
}

int maxFromXs(packet_t *otakuData)
{
    int max = -1;
    for (int i = 0; i < size; i++)
    {
        if (max < otakuData[i].x)
        {
            max = otakuData[i].x;
        }
    }

    return max;
}

int countHigherInQueue(packet_t *otakuData)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (i != rank)
        {
            if (ADDITIONAL_LOGGING)
                debug("\totaku number: %d, P: %d", i, otakuData[i].p);
            if (otakuData[i].p > p)
            {
                count++;
            }
            else if (otakuData[i].p == p && i < rank)
            {
                count++;
            }
        }
    }

    return count;
}

int countCuchyOfGreater(packet_t *otakuData)
{
    int cuchy = 0;
    for (int i = 0; i < size; i++)
    {
        if (i != rank)
        {
            if (otakuData[i].p > p)
            {
                cuchy += otakuData[i].m;
            }
        }
    }

    return cuchy;
}

void fillPs(packet_t *otakuData, int value)
{
    for (int i = 0; i < size; i++)
        otakuData[i].p = value;
}

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
    MPI_Send(packet, 1, MPI_PACKET_T, destination, tag, MPI_COMM_WORLD);
    if (freePacket)
    {
        free(packet);
    }
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
