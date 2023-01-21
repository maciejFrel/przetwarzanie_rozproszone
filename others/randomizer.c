#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <mpi.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define RANDOMIZER 0
#define MONITOR 1

#define RTIME 1
#define TRANSFER 2
#define GIVE_YOUR_STATE 3
#define MY_STATE_IS 4
#define FINISH 5
#define WAIT_SOME 6

#define IDLE 1 
#define TRANSFER 2 

int main(int argc,char **argv)
{
    MPI_Init(&argc, &argv);

    int size,i=0, data;
    int rank;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    /* Dwa pierwsze procesy nie uczestniczą w pracy - 0 to randomizer, 1 to monitor
       mający wykrywać stan kasy w systemie. Kasy powinno byc (size-1)*1000. 
       W chwili obecnej niemal na pewno monitor wykryje liczbę mniejszą
    */
    if (rank==RANDOMIZER) {
            /* RANDOMIZER; szybka i dość toporna metoda udająca "sleep" ;
               niepotrzebna, jeżeli MPI wspiera wątki */
    printf("randomizer started\n");
        while (1) {
	    MPI_Recv( &data, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_TAG==FINISH) {
		printf("RANDOMIZER %d KONIEC!\n", rank);
                break;
            } else 
	    //usleep(1500);
 //           sleep(1);
	    MPI_Send( &data, 1, MPI_INT, status.MPI_SOURCE, RTIME, MPI_COMM_WORLD);
        }
    } else if (rank==MONITOR) {
            /* MONITOR; Jego zadaniem ma być wykrycie, ile kasy jest w systemie */

            // 10 sekund, coby procesy zdążyły namieszać w stanie globalnym
        sleep(10);
            // TUTAJ WYKRYWANIE STANu        
        for (i=2;i<size;i++) 
	    MPI_Send( &data, 1, MPI_INT, i, GIVE_YOUR_STATE, MPI_COMM_WORLD);
        int suma=0;

        for (i=2;i<size;i++)  {
            MPI_Recv( &data, 1, MPI_INT, MPI_ANY_SOURCE, MY_STATE_IS, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
            suma += data;
        }
        
        /* W ogólności wynik będzie błędny (część kasy jest w kanałach)  */
        printf(" Suma %d \n", suma);

	MPI_Send( &data, 1, MPI_INT, RANDOMIZER, FINISH, MPI_COMM_WORLD);
        for (i=2;i<size;i++)  
	    MPI_Send( &data, 1, MPI_INT, i, FINISH, MPI_COMM_WORLD);
        printf("MONITOR KONIEC \n", suma);

    } else {
        int balance = 1000;
        int state = IDLE;
        int who;
	char sleeping = 0;
	int waitingbalance = 0;

        srand(rank);
        MPI_Send( &data, 1, MPI_INT, RANDOMIZER, WAIT_SOME, MPI_COMM_WORLD);

        while ( 1 ) {
	MPI_Recv( &data, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
   
           switch (status.MPI_TAG) {
                case FINISH: goto end; break;
                case GIVE_YOUR_STATE:
		    MPI_Send( &balance, 1, MPI_INT, MONITOR, MY_STATE_IS, MPI_COMM_WORLD);
                break;
                case TRANSFER:
			if (!sleeping) {
                    balance+= data;
#ifdef DEBUG2
            // %c[%d;%dm - 27 == ustawiam atrybuty, 1==bold, 30-black (kolory do 37), potem jest kolo tła (m to IIRC białe, ale nie pamiętam dokładnie)
            printf("%c[%d;%dm [DEBUG %d]: dostalem %d od %d%c[%d;%dm\n", 27, 1, 31+(rank)%6, \
                    rank, data, status.MPI_SOURCE \
                , 27, 0, 30 );
#endif
		} else {
			waitingbalance += data;
		}
                break;
                case RTIME:
                    if (balance > 0 ) {
			data = random() %100;
                        if (data>balance) data=balance;
			balance -= data;
                        
			// nie wysyłam sam do siebie, pętla unika wybranie who==rank
			for (who=rank; who==rank; who = random()%(size-2)+2);
    #ifdef DEBUG2
		printf("%c[%d;%dm[DEBUG %d]: wysylam %d do %d%c[%d;%dm\n",27, 1, 31+(rank)%6, \
		    rank, data, who \
		    , 27, 0, 30 );
    #endif
			MPI_Send( &data, 1, MPI_INT, who, TRANSFER, MPI_COMM_WORLD);
			// toporny "sleep"
                    }
			if (waitingbalance) {
				balance+=waitingbalance;
				waitingbalance =0;
			}
			MPI_Send( &data, 1, MPI_INT, RANDOMIZER, WAIT_SOME, MPI_COMM_WORLD);
			sleeping=1;
               break; 
           };
        };
        end: ;
        printf("%d KONIEC!\n", rank);
    }
    /* wszyscy grzecznie czekamy, aż wszyscy skonczymy. Niepotrzebne, bardziej
       dla demonstracji działania funkcji */
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
