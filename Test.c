#include "Data.h"

/*
Gia na epitaxinw tin diadikasia stis theseis afou ine sinexomenes kai stin idia seira:
An to j >= seatsToReserve tote psaxe epomeni seira min xaneis xrono

0 1 2 3 4 5 6 7 8 9 
* * * * * * . . . .		j=5 Nseat=10 res=5 counter=0
* * * * * + . . . .		j=5 Nseat=10 res=5 counter=1

res - counter < = Nseat-j:
	spase

if(j-seatCounter-seatToReserve>0){
	continue;
}

*/
int** arr;
routine(){
	//printf("Mpika");
	arr[0][1]=1;
	arr[0][2]=1;
	arr[0][3]=1;
	arr[0][4]=1;
	arr[0][5]=1;
	return 1;
}

int main(int argc, char** argv){
	//allocate outer array
	arr = malloc(sizeof(int*)*10); //Ncust
	//allocate inner arrays
	for(int i = 0 ;i <10;i++){
		arr[i] = malloc(sizeof(int*)*7);
	}

	for (int i =0;i<10;i++){
		for (int j =0;j<7;j++){
			arr[i][j] = 0;
		}
	}
	routine();
	for (int i =0;i<10;i++){
		printf("\n");
		for (int j =0;j<7;j++){
			printf("%d",arr[i][j]);
		}
	}
	printf("\n");
	
	//---------------------FREE ARRAY-------------------
	//Deallocate subarrays
	for(int i = 0 ;i <10;i++){
		free(arr[i]);
		arr[i]=NULL;
	}
	//deallocate outer array
	free(arr);
	arr = NULL;




/*-----------PINAKAS PLAN----------------------

rows:Customers
cols:
0.Zone
1.Row
2.Service Time
3.Waiting Time
4.-8.Seats


*/





	/*int arr[30][10];
	int seatToReserve = 5;
	int seatCounter=0;
	
	for (int i = 0; i < 30; i++){
		for (int j = 0; j<10;j++){
			arr [i] [j] = 0;
		}
	}

	for (int j = 0;j<6;j++){
		arr[0][j] = 1;
		arr[1][j] = 1;
	}
	printf("Arxikos pinakas \n");
	for (int i = 0; i < 30; i++){
		printf("\n");
		for (int j = 0; j<10;j++){
			printf("%d",arr [i] [j]);
		}
	}
	printf("\n");
	printf("Mpainw\n");
	int rowCounter = 0;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j<10;j++){
			if(rowCounter!=i){
				seatCounter = 0;
				rowCounter = i;
			}
			if(j-seatCounter-seatToReserve>0){
				//break;
				printf("OOOOPS j: %d\n",j);
				continue;
				printf("PAME\n");
				printf("i: %d, j: %d\n",i,j);
				printf("Condition: %d\n",j-seatCounter-seatToReserve);
				printf("j: %d",j);
			}
			
			if(arr [i][j] == 0){
				arr[i][j] = 1;
				seatCounter++;
			}
		}
	}
	printf("Telikos pinakas\n");
	for (int i = 0; i < 30; i++){
		printf("\n");
		for (int j = 0; j<10;j++){
			printf("%d",arr [i] [j]);
		}
	}
	printf("\n");*/
	/*
	TODO

	1.Timers		(ALMOST DONE)
	2.rc handler
	3. synthiki me j kai seats	(DONE)
	4.Simazema kwdika
	5.ta output tou main nimatos
	Zwni <> / Seira <> / Thesi<> / Pelatis <>
	ara thelw katholiko pinaka Theseis Kleismenes rows 4 cols

	Krataw zone tin zwni / rowCounter tin seira / seatsIndex[] tis theseis / custID o pelatis


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