#include "Data.h"
 

unsigned int seed;



void* routine(void* arg){
	int custID = *(int*)arg;
	int seatsZone;
	int seatsToReserve;
	int flag;	
	int seatCounter,rowCounter;						
	int start,finish;
	unsigned int seatSeed = seed + time(NULL);

	struct timespec startTime;
	flag = 0;
	clock_gettime(CLOCK_REALTIME, &startTime);
	start = startTime.tv_sec;
	seatsToReserve = rand_r(&seatSeed) % NseatLow + NseatHigh;
	seatsZone = 1;							// 0 = A 1 = B
	//printf("SeatSeed %d\n",seatSeed);
	//printf("Seats to reserve %d\n",seatsToReserve);
	//printf("Seats Zone%d",seatsZone);
	pthread_mutex_lock(&mutexAvailableTel);
	while(availableTel == 0){
		pthread_mutex_lock(&screenMutex);
		printf("Customer %d .Sorry all our operators are currently busy. Please wait...\n",custID);
		pthread_mutex_unlock(&screenMutex);
		pthread_cond_wait(&condAvailableTel, &mutexAvailableTel);
	}
	printf("An operator became available. We are connecting you. \n");
	availableTel--;
	pthread_mutex_unlock(&mutexAvailableTel);
	pthread_mutex_lock(&mutexTheaterTable);
	seatCounter = 0;
	int seatsIndex [seatsToReserve];
	if (seatsZone == 0 /*A*/){
		for (int i = 0; i < NzoneA; i++){
			for (int j = 0; j < Nseat; j++){
				if(rowCounter!=i){
					seatCounter = 0;
					rowCounter = i;
				}
				if (theater[i][j] == 0){
					seatCounter ++;
					seatsIndex[seatCounter] = j;
					if(seatCounter == seatsToReserve){
						flag = 1;
						for(int col = 0;col<seatCounter;col++){
							theater[rowCounter][col] = 1;
						}
						goto jmp;
					}
				}
				pthread_mutex_unlock(&mutexTheaterTable);
				pthread_cond_wait(&condTheaterTable, &mutexTheaterTable);
				
			}
		}	
		
	}else {
		for (int i = NzoneA; i < NzoneB; i++){
			for (int j = 0; j < Nseat; j++){
				if(rowCounter!=i){
					seatCounter = 0;
					rowCounter = i;
				}
				if (theater[i][j] == 0){
					seatCounter ++;
					if(seatCounter == seatsToReserve){
						flag = 1;
						for(int col = 0;col<seatCounter;col++){
							theater[rowCounter][col] = 1;
						}
						goto jmp;
					}
				}
				pthread_mutex_unlock(&mutexTheaterTable);
				pthread_cond_wait(&condTheaterTable, &mutexTheaterTable);
				
			}
		}
	}
	pthread_mutex_unlock(&mutexTheaterTable);
	pthread_cond_signal(&condTheaterTable);
	jmp:
		if(flag == 0){
			printf("Customer's %d transaction failed because there are no available seats.\n",custID);
		}else{
			availableTel++;
			pthread_cond_signal(&condAvailableTel);
			printf("Seat counter jmp: %d\n", seatCounter);
			printf("Seats asked: %d\n",seatsToReserve);
			printf("Eureka!!! %d\n",custID);
			printf("Pinakas meta ton %d\n",custID);
			for (int i = 0;i<NzoneA + NzoneB;i++){
				printf("\n");
				for (int j = 0;j<Nseat;j++){
					printf("%d",theater[i][j]);
				}
			}
		}
		

	
}

 
int main(int argc, char** argv) {
	//-------------------------Input-------------------------

	int Ncust;
	printf("Please enter the number of clients.\n");
	scanf("%i", &Ncust);
	if(Ncust<0){
		printf("Error: The number of clients should be greater than or equal to 0. \n");
		exit(-1);
	}
	printf("Please enter the value of the seed for the random number generator.\n");
	scanf("%d", &seed);
	printf("\n\nClients: %i\nSeed: %d\n\n\n", Ncust, seed);

	//-------------------------Variable Declaration-------------------------

	theater [NzoneA + NzoneB] [Nseat];
	for(int i = 0; i < NzoneA + NzoneB; i++){
		for(int j =0; j < Nseat; j++){
			theater[i][j] = 0;
		}
	}

	for (int i = 0;i<NzoneA + NzoneB;i++){
		printf("\n");
		for (int j = 0;j<Nseat;j++){
			printf("%d",theater[i][j]);
		}
				
	}

	bankAccount = 0;
	availableTel = Ntel;
	availableCashiers = Ncash;
	transactionsAttempted = 0;
	successfullTransactions = 0;
	unsuccessfullTransactionsSeats = 0;
	unsuccessfullTransactionsCard = 0;
	totalTransactionCost = 0;
	waitingTime = 0;
	serviceTime = 0;

	//-------------------------Thread Creation-------------------------
	int customer_id[Ncust];
	pthread_t th[Ncust];
	for(int i = 0; i < Ncust; i++){
		customer_id[i] = i + 1;
		printf("Customer %d is calling...\n", customer_id[i]);
		if(pthread_create(&th[i], NULL, &routine, &customer_id[i]) != 0){
			perror("Failed to create thread");
			return 1;
		}
		double randomCallTimer = rand_r(&seed) % TresHigh + TresLow;
		sleep(randomCallTimer);
	}
	for(int i = 0; i < Ncust; i++){
		if(pthread_join(th[i], NULL) != 0){
			perror("Failed to join thread");
			return 2;
		}
	}
	printf("PINAKAS AFTER\n\n");
	for (int i = 0;i<NzoneA + NzoneB;i++){
		printf("\n");
		for (int j = 0;j<Nseat;j++){
			printf("%d",theater[i][j]);
		}
				
	}

	//-------------------------Output-------------------------

	//-------------------------Mutex Destruction-------------------------
	
	pthread_mutex_destroy(&mutexAvailableTel);

	pthread_mutex_destroy(&mutexAvailableCashiers);

	pthread_mutex_destroy(&mutexConsole);

	pthread_mutex_destroy(&mutexBankAcc);
	
	pthread_mutex_destroy(&mutexTheaterTable);

	//-------------------------Conditions Destruction-------------------------

	pthread_cond_destroy(&condAvailableTel);

	pthread_cond_destroy(&condAvailableCashiers);

	return 0;
}
