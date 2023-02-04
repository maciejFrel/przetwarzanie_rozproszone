#include "otaku.h"
#include "main_thread.h"

void mainLoop()
{
    srandom(rank);
    int tag;
    long sleepTime = 1;

    while (1)
    {
        sleepTime = (random() % 4) + 1.0;
        debug("sleep time: %ld", sleepTime);
        sleep(sleepTime);
        // debug("EEEEEEE %lu %d %d", clock(), rand(), random());
        if (state == Out)
        {
            // try to enter the room (send REQ to other otaku)
            packet_t *packet = malloc(sizeof(packet_t));
            packet->p = 0;
            packet->m = 0;
            packet->x = 0;
            debug("\ttrying to get in");
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
