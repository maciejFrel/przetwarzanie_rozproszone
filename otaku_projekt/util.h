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

// get max P from otakus data
int maxFromPs(packet_t *otakuData);

// get max X from otakus data
int maxFromXs(packet_t *otakuData);

// count otakus that have P greater than mine
int countHigherInQueue(packet_t *otakuData);

// count cuchy of otakus that have P greater than mine
int countCuchyOfGreater(packet_t *otakuData);

// fill otakuData with P value
void fillPs(packet_t *otakuData, int value);

#endif
