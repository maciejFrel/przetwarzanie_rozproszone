#include "otaku.h"
#include "main_thread.h"

void mainLoop()
{
    srandom(rank);
    int tag;

    // while (1)
    // {
    if (state == Out)
    {
        packet_t *packet = malloc(sizeof(packet_t));
        packet->p = 0;
        packet->m = 0;
        packet->x = 0;
        for (int i = 0; i < size; i++)
        {
            if (i != rank)
            {
                sendPacket(packet, i, Req);
            }
        }
    }

    // sleep((rand() % 4) + 1);
    // break;
    // }
}
