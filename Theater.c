#include "Data.h"
 
int** plan;				//pointer to the array that will print the theater plan
//int* waitingTimeArray;	//pointer to the array that will hold the waiting times for all the customers
//int* serviceTimeArray;	//pointer to the array that will hold the service times for all the customers

unsigned long seed;
int availableCashiers = Ncash;
int availableTel = Ntel;
int bankAccount = 0;
double transactionsAttempted = 0;
double successfullTransactions = 0;
double unsuccessfullTransactionsSeats = 0;
double unsuccessfullTransactionsCard = 0;
int totalTransactionCost = 0;
double waitingTime = 0;
double serviceTime = 0;


void* routine(void* arg){
	int custID = *(int*)arg;
	int seatsZone;
	int seatsToReserve;
	int flag;	
	int seatCounter,rowCounter;						
	double start,finish,startWaitCashier,finishWaitCashier,finishWaitOperator;
	int cardAccepted;					//Card state. 0
	seatCounter = 0;					//Counter to make sure the seats are continuous
	int startingIndex = 0;				//Index to iterate in search of the seats
	int endingIndex = 0;				//Index to iterate in search of the seats
	int seatsIndex [seatsToReserve];	//Array to hold the index of the seats to be reserved
	int ticketsCost = 0;				//Cost of the tickets
	double randomTimer ;				//Random amount of time to sleep
	flag = 0;							//flag to check whether the seats have been found already or not
	char zone;							//A char to help us print the zone (A , B) instead of (0 , 1)
	unsigned long seatSeed;				//Seed for the seat selection to get more random results
	unsigned long zoneSeed;				//Seed for the seat selection to get more random results
	unsigned long cardSeed;				//Seed for the card to either go through or not and get more random results
	unsigned long waitSeed;				//Seed for waiting time to get more random results

	struct timespec startTimeService;	//Starting time of the whole process of reserving the tickets
	struct timespec	endTimeService;		//Ending time of the whole process of reserving the tickets
	struct timespec	startTimeWaitingCashier;	//Starting time of the customer waiting for an available operator/cashier
	struct timespec	endTimeWaitingCashier;		//Ending time of the customer waiting for an available cashier
	struct timespec	endTimeWaitingOperator;		//Ending time of the customer waiting for an available operator

	clock_gettime(CLOCK_REALTIME, &startTimeService);
	start = startTimeService.tv_sec;

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
	clock_gettime(CLOCK_REALTIME, &endTimeWaitingOperator);
	finishWaitOperator = endTimeWaitingOperator.tv_sec;
	//printf("Waiting time for operator: %0.02f\n",finishWaitOperator-start);
	availableTel--;
	pthread_mutex_unlock(&mutexAvailableTel);
	seatSeed = seed + time(NULL);
	zoneSeed = seed + time(NULL);
	seatsToReserve = rand_r(&seatSeed) % NseatHigh + NseatLow;			//Amount of seats the client wants
	seatsZone = rand_r(&zoneSeed) % 100 / 100.0f > PzoneA;				// Variable to hold the prefered zone. 0 = A 1 = B
	pthread_mutex_lock(&screenMutex);
	printf("ATTENTION!!!! Customer %d wants zone %d and %d seats\n",custID,seatsZone,seatsToReserve);
	pthread_mutex_unlock(&screenMutex);
	if(seatsZone == 0){
        rowCounter = 0;
        startingIndex = 0;
        endingIndex = NzoneA;
		zone = 'A';
    }else{
        rowCounter = NzoneA;
        startingIndex = NzoneA;
        endingIndex = NzoneA + NzoneB;
		zone = 'B';
    }
	waitSeed = seed + time(NULL);
	randomTimer = rand_r(&waitSeed) % TseatHigh + TseatLow;			//Amount of time that the operator needs to check for the seats
	sleep(randomTimer);	
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
				if(j-seatCounter+seatsToReserve>Nseat){				//This is to save time.We dont have to check the whole row if the remaining seats are less than the ones we need
					continue;										//Continue searching on the next row
				}
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
		if(flag == 0){												//Checking if the operator found available seats. 0 = No 1 = Yes
			pthread_mutex_lock(&screenMutex);
			printf("Customer's %d transaction failed because there are no available seats.\n",custID);
			pthread_mutex_unlock(&screenMutex);
			unsuccessfullTransactionsSeats++;
			pthread_mutex_lock(&mutexAvailableTel);
			availableTel++;											//releasing the operator
			pthread_cond_signal(&condAvailableTel);
			pthread_mutex_unlock(&mutexAvailableTel);
		}else{
			pthread_mutex_lock(&screenMutex);
			printf("Pinakas meta ton %d\n",custID);
			for (int i = 0;i<NzoneA + NzoneB;i++){
				printf("\n");
				for (int j = 0;j<Nseat;j++){
					printf("%d",theater[i][j]);
				}
			}
			pthread_mutex_unlock(&screenMutex);
			pthread_mutex_lock(&mutexAvailableTel);
			availableTel++;											//releasing the operator
			pthread_cond_signal(&condAvailableTel);
			pthread_mutex_unlock(&mutexAvailableTel);

			clock_gettime(CLOCK_REALTIME, &startTimeWaitingCashier);
			startWaitCashier = startTimeWaitingCashier.tv_sec;

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

			clock_gettime(CLOCK_REALTIME, &endTimeWaitingCashier);
			finishWaitCashier = endTimeWaitingCashier.tv_sec;
			printf("Waiting time for cashier: %0.02f\n",finishWaitCashier-startWaitCashier);

			waitSeed = seed + time(NULL);
			randomTimer = rand_r(&waitSeed) % TcashHigh + TcashLow;			//Amount of time that the cashier needs to try the card
			sleep(randomTimer);	

			cardSeed = seed + time(NULL);
			cardAccepted = rand_r(&cardSeed) % 100 / 100.0f > PcardSuccess;
			if(cardAccepted == 0){											//if card gets accepted
				pthread_mutex_lock(&screenMutex);
				printf("Customer %d\n",custID);
				printf("The transaction is completed!. Your seats are: zone <%c>, row <%d>,number <",zone,rowCounter);
				for(int a=0;a<seatCounter;a++){
					printf("%d,",seatsIndex[a]);
					plan[custID-1][a+4] = seatsIndex [a];
				}
				printf("> and the total cost is <%d> euro.\n",ticketsCost);
				pthread_mutex_unlock(&screenMutex);

				plan [custID-1][0] = seatsZone;
				plan [custID-1][1] = rowCounter;
				

				successfullTransactions++;
				pthread_mutex_lock(&mutexBankAcc);
				bankAccount+=ticketsCost;
				pthread_mutex_unlock(&mutexBankAcc);
			}else{															//if card gets declined
				pthread_mutex_lock(&screenMutex);
				printf("Customer %d\n",custID);
				printf("The transaction failed because your card was declined.\n");
				pthread_mutex_unlock(&screenMutex);
				unsuccessfullTransactionsCard++;

				pthread_mutex_lock(&mutexTheaterTable);
				for(int col = 0;col<seatCounter;col++){
					theater[rowCounter][seatsIndex[col]] = 0;		//Freeing the seats that were to be reserved but the transaction failed
				}
				pthread_mutex_unlock(&mutexTheaterTable);
			}
			availableCashiers++;
			pthread_cond_signal(&condAvailableCashiers);

		}
		clock_gettime(CLOCK_REALTIME, &endTimeService);
		finish = endTimeService.tv_sec;
		serviceTime = finish - start;
		waitingTime = (finishWaitCashier - startWaitCashier) + (finishWaitOperator - start);
		plan [custID-1][2] = serviceTime;
		plan [custID-1][3] = waitingTime;
		pthread_mutex_lock(&screenMutex);
		printf("Customer %d, waited a total of %0.00f seconds\n",custID,serviceTime);
		pthread_mutex_unlock(&screenMutex); 
		pthread_exit(NULL);	
		

	
}

 
int main(int argc, char** argv) {
	//-------------------------Input-------------------------

	int Ncust;
	unsigned long seedSleep;
	printf("Please enter the number of clients.\n");
	scanf("%i", &Ncust);
	if(Ncust<0){
		printf("Error: The number of clients should be greater than or equal to 0. \n");
		exit(-1);
	}
	printf("Please enter the value of the seed for the random number generator.\n");
	scanf("%ld", &seed);
	printf("\n\nClients: %i\nSeed: %ld\n\n\n", Ncust, seed);

	//-------------------------Variable Declaration-------------------------
	int *planForOutput  = malloc(Ncust*9*sizeof(int));
	
	double waitingTimeSum = 0;								//For the statistics
	double serviceTimeSum = 0;								//For the statistics

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
	//--------------------------------------Array for outputing the theater plan---------------------------------------------
	//allocate outer array
	plan = malloc(sizeof(int*)*Ncust);
	//allocate inner arrays
	for(int i = 0 ;i < Ncust ;i++){
		plan[i] = malloc(sizeof(int*)*7);
	}

	for (int i =0;i< Ncust ;i++){
		for (int j =0;j<9;j++){
			plan[i][j] = -1;
		}
	}
	/*
	printf("ARXIKO PLANO\n")
	for (int i =0;i< Ncust ;i++){
		printf("\n")
		for (int j =0;j<7;j++){
			prinf("%d",plan[i][j])
		}
	}*/

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
		seedSleep = rand_r(&seed) + time(NULL);
		double randomCallTimerMain = rand_r(&seedSleep) % TresHigh + TresLow;			//Random number of seconds untill next client calls
		sleep(randomCallTimerMain);
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
	printf("\n");
	transactionsAttempted = successfullTransactions + unsuccessfullTransactionsCard + unsuccessfullTransactionsSeats;
	for (int i = 0;i<Ncust;i++){
		serviceTimeSum += plan[i][2];
		waitingTimeSum += plan[i][3];
	}
	//-------------------------Output-------------------------
	printf("The total income from the ticket sales is %d\n",bankAccount);
	printf("Total transactions attempted: %0.02f\n",transactionsAttempted);
	printf("Successfull transactions percentage: %0.02f\n",successfullTransactions / transactionsAttempted);
	printf("Unsuccessfull transactions due to card percentage: %0.02f\n",unsuccessfullTransactionsCard / transactionsAttempted);
	printf("Unsuccessfull transactions due to seats percentage: %0.02f\n",unsuccessfullTransactionsSeats / transactionsAttempted);
	printf("Successfull transactions: %0.02f\n",successfullTransactions );
	printf("Unsuccessfull transactions due to card: %0.02f\n",unsuccessfullTransactionsCard );
	printf("Unsuccessfull transactions due to seats: %0.02f\n",unsuccessfullTransactionsSeats);
	//printf("Service time sum: %d seconds\n",serviceTimeSum);
	printf("Average service time: %0.02f seconds\n",serviceTimeSum/Ncust);
	printf("Waiting time sum: %d seconds\n",waitingTimeSum);
	printf("Average waiting time: %0.02f seconds\n",waitingTimeSum/Ncust);

	for (int i =0;i<Ncust;i++){
		if(plan[i][0]==-1){
			continue;
		}
		if(plan[i][0] == 0){
			printf("Customer <%d> Zone <%c> Row<%d> Seats<",customer_id[i],'A',plan[i][1]);
		}else{
			printf("Customer <%d> Zone <%c> Row<%d> Seats<",customer_id[i],'B',plan[i][1]);
		}
		
		for (int j =4;j<9;j++){
			if(plan[i][j]!=-1){
				printf("%d,",plan[i][j]);
			}
			
		}
		printf(">\n");
	}

	//Deallocate subarrays
	for(int i = 0 ;i <Ncust;i++){
		free(plan[i]);
		plan[i]=NULL;
	}
	//deallocate outer array
	free(plan);
	plan = NULL;


	//-------------------------Mutex Destruction-------------------------
	
	pthread_mutex_destroy(&mutexAvailableTel);

	pthread_mutex_destroy(&mutexAvailableCashiers);

	pthread_mutex_destroy(&mutexBankAcc);
	
	pthread_mutex_destroy(&mutexTheaterTable);

	//-------------------------Conditions Destruction-------------------------

	pthread_cond_destroy(&condAvailableTel);

	pthread_cond_destroy(&condAvailableCashiers);

	pthread_cond_destroy(&condTheaterTable);

	return 0;
}
