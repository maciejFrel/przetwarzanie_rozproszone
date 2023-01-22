#ifndef MAINH
#define MAINH
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>

/* boolean */
#define TRUE 1
#define FALSE 0

#define ROOT 0

/* typ pakietu */
typedef struct
{
    int src;

    int data; /* przykładowe pole z danymi; można zmienić nazwę na bardziej pasującą */
} packet_t;
/* packet_t ma dwa pola, więc NITEMS=3. Wykorzystane w inicjuj_typ_pakietu */
#define NITEMS 2

/* Typy wiadomości */
#define APP_PKT 1

MPI_Datatype MPI_PAKIET_T;

void inicjuj_typ_pakietu();

#endif
