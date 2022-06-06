#pragma once
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
 
//--------------------------------------Constants--------------------------------------
 
#define Ntel 3					//number of phone operators
 
#define Ncash 2					//number of cashiers
 
#define Nseat 10				//number of seats per row
 
#define NzoneA 10				//number of rows in the A zone
 
#define NzoneB 20				//number of rows in the B zone
 
#define PzoneA 0.3		        //propability of choosing zone A
 
#define PzoneB 1 - PzoneA		//propability of choosing zone B
 
#define CzoneA 30				//cost of a zone A seat
 
#define CzoneB 20				//cost of a zone B seat
 
#define NseatLow 1				//Minimun number of seats selected by the client
 
#define NseatHigh 5				//Maximum number of seats selected by the client
 
#define TresLow 1				//Minimum amount of seconds needed for the next client to make a call
 
#define TresHigh 5				//Maximum amount of seconds needed for the next client to make a call
 
#define TseatLow 5				//Minimum amount of seconds needed for the operator to check for the availability of the seats
 
#define TseatHigh 13			//Maximum amount of seconds needed for the operator to check for the availability of the seats
 
#define TcashLow 4				//Minimum amount of seconds needed for the operator to validate the payment
 
#define TcashHigh 8				//Maximum amount of seconds needed for the operator to validate the payment
 
#define PcardSuccess 0.9	    //Propability that the payment will go through
 
//--------------------------------------Variables--------------------------------------

int theater [NzoneA + NzoneB] [Nseat];  //A depiction of how the seats in the theater are organised. 0 = free and 1 = reserved

int bankAccount;                        //Bank account of the system

int availableTel;                       //Available phone operators

int availableCashiers;                  //Available cashiers

double transactionsAttempted;              //Total amount of transactions attempted

double successfullTransactions;            //Total amount of successfull transactions

double unsuccessfullTransactionsSeats;     //Total amount of unsuccessfull transactions due to seats

double unsuccessfullTransactionsCard;      //Total amount of unsuccessfull transactions due to card being declined

int totalTransactionCost;               //Total transaction cost

double waitingTime;                     //Waiting time where the clients waits to talk to someone

double serviceTime;                     //Amount of time from when the client shows up until he finishes the interaction

//--------------------------------------Mutexes--------------------------------------

pthread_mutex_t mutexAvailableTel = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutexAvailableCashiers = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutexBankAcc = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutexTheaterTable = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t screenMutex = PTHREAD_MUTEX_INITIALIZER;


//--------------------------------------Conditions--------------------------------------

pthread_cond_t condAvailableTel = PTHREAD_COND_INITIALIZER;

pthread_cond_t condAvailableCashiers = PTHREAD_COND_INITIALIZER;

pthread_cond_t condTheaterTable = PTHREAD_COND_INITIALIZER;

//--------------------------------------Functions--------------------------------------

unsigned int sleep (unsigned int seconds);