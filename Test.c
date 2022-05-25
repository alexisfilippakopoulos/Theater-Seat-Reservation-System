#include "Data.h"

/*
Gia na epitaxinw tin diadikasia stis theseis afou ine sinexomenes kai stin idia seira:
An to j >= seatsToReserve tote psaxe epomeni seira min xaneis xrono

0 1 2 3 4 5 6 7 8 9 
* * * * * * . . . .		j=5 Nseat=10 res=5 counter=0
* * * * * + . . . .		j=5 Nseat=10 res=5 counter=1

res - counter < = Nseat-j:
	spase


*/

int main(int argc, char** argv){
	unsigned long seatSeed = 1000;
	unsigned long seed;
	int seatsToReserve;

	seed = seatSeed + time(NULL);
	printf("New seed %ld\n",seed);
	sleep(10);

	int cardAccepted;
	cardAccepted = rand_r(&seed) % 100 / 100.0f > PzoneA;
	printf("Res %d\n",cardAccepted);

	seed = seatSeed + time(NULL);
	printf("New seed %ld\n",seed);
	sleep(10);


	cardAccepted = rand_r(&seed) % 100 / 100.0f > PzoneA;
	printf("Res %d\n",cardAccepted);

	seed = seatSeed + time(NULL);
	printf("New seed %ld\n",seed);
	sleep(10);


	cardAccepted = rand_r(&seed) % 100 / 100.0f > PzoneA;
	printf("Res %d\n",cardAccepted);

	seed = seatSeed + time(NULL);
	printf("New seed %ld\n",seed);
	sleep(5);


	cardAccepted = rand_r(&seed) % 100 / 100.0f > PzoneA;
	printf("Res %d",cardAccepted);
	

	seed = seatSeed + time(NULL);
	printf("New seed %ld\n",seed);
	sleep(10);


	cardAccepted = rand_r(&seed) % 100 / 100.0f > PzoneA;
	printf("Res %d\n",cardAccepted);

	seed = seatSeed + time(NULL);
	printf("New seed %ld\n",seed);
	sleep(10);


	cardAccepted = rand_r(&seed) % 100 / 100.0f > PzoneA;
	printf("Res %d\n",cardAccepted);

	seed = seatSeed + time(NULL);
	printf("New seed %ld\n",seed);
	sleep(5);


	cardAccepted = rand_r(&seed) % 100 / 100.0f > PzoneA;
	printf("Res %d",cardAccepted);
	
	
	
	/*
	TODO

	1.Timers
	2.rc handler
	3. synthiki me j kai seats

	*/
	
	
	
	
	



	/*
	int res = 5;

	int ar [NzoneA + NzoneB] [Nseat];
	for(int i = 0; i < NzoneA + NzoneB; i++){
		for(int j =0; j < Nseat; j++){
			ar[i][j] = 0;
		}
	}

	for (int a = 0 ; a < 6; a++){
		ar[0][a] = 1;
		ar[1][a] = 1;
	}

	for(int i = 0; i < NzoneA + NzoneB; i++){
		printf("\n");
		for(int j =0; j < Nseat; j++){
			printf("%d",ar[i][j]);
		}
	}
	printf("\n");
	for(int j =0; j < Nseat; j++){
		if (res -1 + j > 9){
			printf("YES\n");
		}

	}
	*/
    return 0;
}