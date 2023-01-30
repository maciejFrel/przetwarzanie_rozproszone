#include "otaku.h"
#include "main_thread.h"

void mainLoop()
{
    srandom(rank);
    int tag;

    while (1)
    {
        sleep((rand() % 4) + 1);
        // debug("%lu %d", clock(), rand());
        if (state == Out)
        {
            // try to enter the room (send REQ to other otaku)
            packet_t *packet = malloc(sizeof(packet_t));
            packet->p = 0;
            packet->m = 0;
            packet->x = 0;
            // debug("sending REQ");
            for (int i = 0; i < size; i++)
            {
                if (i != rank)
                {
                    sendPacket(packet, i, Req);
                }
            }
        } else if (rand() % 100 < 20) {
            // exit room
            state = Out;
            p = 0;
            x = 0;
            debug("---OUT---");
        }
    }
}
