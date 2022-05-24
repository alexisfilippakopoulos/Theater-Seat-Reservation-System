#include "Data.h"

/*
Gia na epitaxinw tin diadikasia stis theseis afou ine sinexomenes kai stin idia seira:
An to j >= seatsToReserve tote psaxe epomeni seira min xaneis xrono
*/

int main(int argc, char** argv){

    if(seatsZone == 0){
        rowCounter = 0;
        start = 0;
        end = NzoneA;
    }else{
        rowCounter = NzoneA;
        start = NzoneA;
        end = NzoneA + NzoneB;
    }

    for (int i = start; i < end; i++){
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
						goto jmp;
					}
				}
				
			}
		}

    }
    if (seatsZone == 0){									//If client chooses A zone
		rowCounter = 0;
		for (int i = 0; i < NzoneA; i++){
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
						goto jmp;
					}
				}
				
			}
		}	
		
	}else {
		rowCounter = NzoneA;
		for (int i = NzoneA; i < NzoneA + NzoneB; i++){
			for (int j = 0; j < Nseat; j++){
				if(rowCounter!=i){
					for(int i = 0; i<seatCounter;i++){
						seatsIndex[i] = 0;
					}					seatCounter = 0;
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
						goto jmp;
					}
				}
				
            }
        }
    
    }

    return 0;
}