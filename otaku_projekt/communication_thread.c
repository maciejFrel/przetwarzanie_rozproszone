#include "otaku.h"
#include "communication_thread.h"

void *startCommunicationThread(void *ptr)
{
    packet_t *otakuData = malloc(size * sizeof(packet_t)); // dane o pozostałych otaku

    for (int i = 0; i < size; i++)
    {
        otakuData[i].p = -1;
    }
    otakuData[rank].p = 0;
    // debug("p: %d", otakuData[0]);

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
        {
            // debug("I got REQ from %d", status.MPI_SOURCE);
            packet_t *pkt = malloc(sizeof(packet_t));
            pkt->m = m;
            pkt->p = p;
            pkt->x = x;
            sendPacket(pkt, status.MPI_SOURCE, Ack);
            break;
        }
        case Ack:
            // debug("p: %d", otakuData[0].p);
            // debug("I got ACK from %d (m: %d, p: %d, x: %d)", status.MPI_SOURCE, packet.m, packet.p, packet.x);
            otakuData[status.MPI_SOURCE].m = packet.m;
            otakuData[status.MPI_SOURCE].p = packet.p;
            otakuData[status.MPI_SOURCE].x = packet.x;
            break;
        case Release:
            // debug("I got RELEASE from %d", status.MPI_SOURCE);
            x = 0;
            break;
        }

        int f = 1;
        for (int i = 0; i < size; i++)
        {
            if (i != rank)
            {
                // debug("p %d", otakuData[i].p);
                if (otakuData[i].p == -1)
                {
                    f = 0;
                    break;
                }
            }
        }

        if (f)
        {
            if (state == Out) // ACK otrzymane od wszystkich pozostałych otaku
            {
                if (ADDITIONAL_LOGGING)
                    debug("\tI got all ACK");
                int a = countHigherInQueue(otakuData);
                if (ADDITIONAL_LOGGING)
                    debug("\tnumber of otakus higher in queue: %d, x: %d, p: %d", a, x, p);
                if (a < S) // sprawdzanie czy jest miejsce w pomieszczeniu
                {
                    int b = countCuchyOfGreater(otakuData);
                    if (b < M) // sprawdzanie czy nie przekroczymy M
                    {
                        debug("IN\tcuchy: %d, równoczesne cuchy znajdujących sie powyzej w kolejce (M): %d", m, b);
                        // wejście do pomieszczenia
                        state = In;
                        m += rand() % 3 + 1;
                        p = maxFromPs(otakuData) + 1;
                    } else {
                        debug("ŚMIERDZE!!! (przekroczyłbym M)");
                    }
                }
                else
                {
                    int c = maxFromXs(otakuData);
                    if (c + m > X) // sprawdzanie czy trzeba zawiadomić kolejnego przedstawiciela
                    {
                        debug("asking for new representative (sending RELEASE)");
                        for (int i = 0; i < size; i++)
                        {
                            if (i != rank)
                            {
                                sendPacket(0, i, Release); // zawiadomienie nowego przedstawiciela
                            }
                        }
                    }
                }
                fillPs(otakuData, -1); // wyzerowanie lokalnie przechowanych danych z ACK
            }
        }
    }
}
