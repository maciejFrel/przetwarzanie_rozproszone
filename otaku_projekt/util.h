#ifndef UTILH
#define UTILH
#include "otaku.h"

typedef enum
{
    Req = 1,
    Ack,
    Release,
} type_t;

typedef struct
{
    int p; // clock
    int m; // liczba cuchów otaku
    int x; // skumulowana liczba cuchów dla przedstawiciela
} packet_t;

typedef enum
{
    In,
    Out,
} state_t;

#define NITEMS 3

#define APP_PKT 1
#define FINISH 2

extern MPI_Datatype MPI_PACKET_T;
void initializePacketType();

void sendPacket(packet_t *pkt, int destination, int tag);
int maxFromPs(packet_t *otakuData);
int maxFromXs(packet_t *otakuData);
int countGreater(packet_t *otakuData);
int countCuchyOfGreater(packet_t *otakuData);
void fillPs(packet_t *otakuData, int value);

#endif
