#include "otaku.h"
#include "main_thread.h"

void mainLoop()
{
    srandom(rank);
    long sleepTime = 1;

    while (1)
    {
        if (state == Out)
        {
            // try to enter the room (send REQ to other otaku)
            packet_t *packet = malloc(sizeof(packet_t));
            packet->p = 0;
            packet->m = 0;
            packet->x = 0;
            
            // setting P to 1 to differentiate from 0 which is otaku that is not requesting to enter the room
            p = 1;
            
            debug("trying to get in (sending REQ)");
            
            for (int i = 0; i < size; i++)
            {
                if (i != rank)
                {
                    sendPacket(packet, i, Req);
                }
            }
        }
        else if (state == In && random() % 100 < 30)
        {
            // exit room
            state = Out;
            p = 0;
            x = 0;
            debug("OUT");
        }
        sleep(SEC_IN_STATE);
    }
}
