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
	seatsToReserve = rand_r(&seatSeed) % NseatLow + NseatHigh;			//Amount of seats the client wants
	seatsZone = 1;						// Variable to hold the prefered zone. 0 = A 1 = B
	pthread_mutex_lock(&mutexAvailableTel);
	while(availableTel == 0){
		pthread_mutex_lock(&screenMutex);
		printf("Customer %d. All operators are currently busy. Please wait...\n",custID);
		pthread_mutex_unlock(&screenMutex);
		pthread_cond_wait(&condAvailableTel, &mutexAvailableTel);
	}
	pthread_mutex_lock(&screenMutex);
	printf("An operator is now available. Customer %d, we are connecting you. \n",custID);
	pthread_mutex_unlock(&screenMutex);
	availableTel--;
	pthread_mutex_unlock(&mutexAvailableTel);
	seatCounter = 0;					//Counter to make sure the seats are continuous
	int startingIndex = 0;				//Index to iterate in search of the seats
	int endingIndex = 0;				//Index to iterate in search of the seats
	int seatsIndex [seatsToReserve];	//Array to hold the index of the seats to be reserved
	int ticketsCost = 0;				//Cost of the tickets
	if(seatsZone == 0){
        rowCounter = 0;
        startingIndex = 0;
        endingIndex = NzoneA;
    }else{
        rowCounter = NzoneA;
        startingIndex = NzoneA;
        endingIndex = NzoneA + NzoneB;
    }

	//----------------------Ticket Reservation Process------------------------
	pthread_mutex_lock(&mutexTheaterTable);
    for (int i = startingIndex; i < endingIndex; i++){
		for (int j = 0; j < Nseat; j++){
			if(rowCounter!=i){
				for(int i = 0; i<seatCounter;i++){
					seatsIndex[i] = 0;
				}
				seatCounter = 0;
				rowCounter = i;
			}
			if (theater[i][j] == 0){
				seatsIndex[seatCounter] = j;
				seatCounter ++;
				if(seatCounter == seatsToReserve){
					flag = 1;
					for(int col = 0;col<seatCounter;col++){
						theater[rowCounter][seatsIndex[col]] = 1;
					}
					if(seatsZone == 0){
						ticketsCost = CzoneA * seatCounter;
					}else{
						ticketsCost = CzoneB * seatCounter;
					}
					goto jmp;										//if we find the seats there is no need to iterate the rest of the array so we can save time
				}
			}
		}
	}
	
	jmp:
		pthread_mutex_unlock(&mutexTheaterTable);
		if(flag == 0){
			pthread_mutex_lock(&screenMutex);
			printf("Customer's %d transaction failed because there are no available seats.\n",custID);
			pthread_mutex_unlock(&screenMutex);
		}else{
			//pthread_cond_signal(&condTheaterTable);
			pthread_mutex_lock(&screenMutex);
			printf("Seat counter jmp: %d\n", seatCounter);
			printf("Seats asked: %d\n",seatsToReserve);
			printf("Cost: %d\n",ticketsCost);
			pthread_mutex_unlock(&screenMutex);
			/*printf("Pinakas meta ton %d\n",custID);
			for (int i = 0;i<NzoneA + NzoneB;i++){
				printf("\n");
				for (int j = 0;j<Nseat;j++){
					printf("%d",theater[i][j]);
				}
			}*/
			pthread_mutex_lock(&mutexAvailableCashiers);
			while (availableCashiers == 0){
				pthread_mutex_lock(&screenMutex);
				printf("Customer %d. All cashiers are currently busy. Please wait...\n",custID);
				pthread_mutex_unlock(&screenMutex);
				pthread_cond_wait(&condAvailableCashiers, &mutexAvailableCashiers);
			}
			pthread_mutex_lock(&screenMutex);
			printf("A cashier is now available. Customer %d, we are connecting you. \n",custID);
			pthread_mutex_unlock(&screenMutex);
			availableCashiers--;
			pthread_mutex_unlock(&mutexAvailableCashiers);
			pthread_mutex_lock(&mutexAvailableTel);
			availableTel++;											//releasing the operator
			pthread_cond_signal(&condAvailableTel);
			pthread_mutex_lock(&mutexAvailableTel);
			sleep(10);
			printf("Done with cashier!\n");
			availableCashiers++;
			pthread_cond_signal(&condAvailableCashiers);
			

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
	/*
	printf("ARXIKOS PINAKAS\n");
	for (int i = 0;i<NzoneA + NzoneB;i++){
		printf("\n");
		for (int j = 0;j<Nseat;j++){
			printf("%d",theater[i][j]);
		}
				
	}
	*/
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
	printf("PINAKAS TELIKOS\n\n");
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
