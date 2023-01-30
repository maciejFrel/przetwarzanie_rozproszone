#include "otaku.h"
#include "communication_thread.h"

int maxFromPs(int *ps)
{
    int max = -1;
    for (int i = 0; i < size; i++)
    {
        if (max < ps[i])
        {
            max = ps[i];
        }
    }
    
    return max;
}

int countGreaterOrEqual(int *ps, int value) {
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (value >= ps[i])
        {
            count++;
        }
    }
    
    return count;
} 

void fillPs(int *ps, int value)
{
    for (int i = 0; i < size; i++)
    {
        ps[i] = value;
    }
}

void *startCommunicationThread(void *ptr)
{
    int ps[8];
    for (int i = 0; i < size; i++)
    {
        ps[i] = -1;
    }
    ps[rank] = 0;
    // debug("p: %d", ps[0]);

    MPI_Status status;
    packet_t packet;

    while (1)
    {
        // debug("waiting for message");
        MPI_Recv(&packet, 1, MPI_PACKET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        // debug("m%d, p%d, x%d, tag%d, source%d\n", packet.m, packet.p, packet.x, status.MPI_TAG, status.MPI_SOURCE);

        switch (status.MPI_TAG)
        {
            case Req:
                debug("I got Req from %d", status.MPI_SOURCE);
                packet_t *packet = malloc(sizeof(packet_t));
                packet->m = m;
                packet->p = p;
                packet->x = x;
                sendPacket(packet, status.MPI_SOURCE, Ack);
                break;
            case Ack:
                debug("p: %d", ps[0]);
                debug("I got Ack from %d (m: %d, p: %d, x: %d)", status.MPI_SOURCE, packet->m, packet->p, packet->x);
                ps[status.MPI_SOURCE] = packet->p;
                break;
            case Release:
                debug("I got Release from %d", status.MPI_SOURCE);
                m = 0;
                break;
        }

        // check if I hot all the ACKs i need
        int f = 1;
        for (int i = 0; i < size; i++)
        {
            if (ps[i] == -1)
            {
                f = 0;
                break;
            }
        }

        if (f)
        {
            int a = countGreaterOrEqual(ps, p);
            if (a < S) {
                if ()
            }
            fillPs(ps, -1);
        }
    }
}
