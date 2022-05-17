#include "Data.h"
 

int seed;
int Ncust;

void* routine(void* arg){
	int custID = *(int*)arg;

	pthread_mutex_lock(&mutexAvailableTel);
	while(availableTel == 0){
		printf("Sorry all our operators are currently busy. Please wait...\n");
		pthread_cond_wait(&condAvailableTel, &mutexAvailableTel);
	}
	printf("Got a hold of some1 tyvm UwU....");
	availableTel--;
	pthread_cond_signal(&condAvailableTel);
	pthread_mutex_unlock(&mutexAvailableTel);
}

 
int main(int argc, char** argv) {
	//-------------------------Input-------------------------

	printf("Please enter the number of clients.\n");
	scanf("%i", &Ncust);
	if(Ncust<0){
		printf("Error: The number of clients should be greater than or equal to 0. \n");
		exit(-1);
	}
	printf("Please enter the value of the seed for the random number generator.\n");
	scanf("%i", &seed);
	printf("\n\nClients: %i\nSeed: %i\n\n\n", Ncust, seed);

	//-------------------------Variable Declaration-------------------------

	theater [NzoneA + NzoneB] [Nseat];
	for(int i = 0; i < NzoneA + NzoneB; i++){
		for(int j =0; j < Nseat; j++){
			theater [i] [j] = 0;
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
