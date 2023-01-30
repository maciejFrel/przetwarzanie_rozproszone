#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "util.h"

typedef struct
{
    int type; // 1 - REQ, 2 - ACK, 3 - RELEASE and only for ACK data has to be sent (p, m, x)
    int p; // clock
    int m; // liczba cuchów otaku
    int x; // skumulowana liczba cuchów dla przedstawiciela
} packet_data;

typedef enum
{
    In,
    Out,
} state;