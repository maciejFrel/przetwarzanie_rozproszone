#include "main.h"
/* tworzy typ MPI_PAKIET_T
 */
void inicjuj_typ_pakietu()
{
    /* Stworzenie typu */
    /* Poniższe (aż do MPI_Type_commit) potrzebne tylko, jeżeli
       brzydzimy się czymś w rodzaju MPI_Send(&typ, sizeof(pakiet_t), MPI_BYTE....
    */
    /* sklejone z stackoverflow */
    int blocklengths[NITEMS] = {1, 1};
    MPI_Datatype typy[NITEMS] = {MPI_INT, MPI_INT};

    MPI_Aint offsets[NITEMS];
    offsets[0] = offsetof(packet_t, src);
    offsets[1] = offsetof(packet_t, data);

    MPI_Type_create_struct(NITEMS, blocklengths, offsets, typy, &MPI_PAKIET_T);
    MPI_Type_commit(&MPI_PAKIET_T);
}

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    srand(rank);
    inicjuj_typ_pakietu(); // tworzy typ pakietu
    packet_t pkt;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int ile_razy = 0;
    pkt.data = 1;
    if (rank == ROOT)
        MPI_Send(&pkt, 1, MPI_PAKIET_T, 1, APP_PKT, MPI_COMM_WORLD);

    while (TRUE)
    {
        printf("[%d] Czekam na pakiet\n", rank);
        MPI_Recv(&pkt, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        pkt.data++;
        ile_razy++;
        if ((rank == ROOT) && (ile_razy <= 4))
        { // zwróć uwagę na ostrzeżenie przy mpi_run. Czy pojawi się po odkomentowaniu?
            printf("[%d] Wysyłam pakiet %d z %d (%d)\n", rank, ile_razy, pkt.data, ile_razy <= 4);
            MPI_Send(&pkt, 1, MPI_PAKIET_T, (rank + 1) % size, APP_PKT, MPI_COMM_WORLD);
        }
        else if (rank != ROOT)
        {
            printf("[%d] Wysyłam pakiet %d z %d\n", rank, ile_razy, pkt.data);
            MPI_Send(&pkt, 1, MPI_PAKIET_T, (rank + 1) % size, APP_PKT, MPI_COMM_WORLD);
        }
        if (ile_razy > 4)
            break;
    }
    if (rank == ROOT)
        printf("[%d] Koniec %d!\n", rank, pkt.data);

    MPI_Type_free(&MPI_PAKIET_T);
    MPI_Finalize();
    return 0;
}
