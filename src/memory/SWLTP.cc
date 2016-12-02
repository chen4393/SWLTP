/*
*
* M2S SWLTP With LTP Implementation
* Zach Kabitz, Danny Huang, Yu Liu, Chaoran Chen, Apoorva M. Nataraja
* 11/28/2016
*
* The SWLTP class is the implementation of a last-touch (dead-block) predictor. It makes use of 
* a history table to hold generated traces as accesses to memory are made, and a last touch history table
* which is used to compare the currently generated trace for a block to the previous last-held trace in the
* history table when that block was evicted last.
*/


#include "SWLTP.h"

namespace mem
{

SWLTP::SWLTP(unsigned inum_sets, unsigned inum_ways){
	num_sets=inum_sets;
	num_ways=inum_ways;
	HistoryTable=(HistoryInfo**) malloc (sizeof(num_sets*(HistoryInfo*)));
	int i=0; int j=0;
	for(i=0;i<num_ways; i++){
		HistoryTable[i]=(HistoryInfo*) malloc (num_ways*sizeof(HistoryInfo));	
	}

	for (i = 0; i <  num_sets; i++){
		for (j = 0; j < num_ways; j++){
			HistoryTable[i][j].PC =0;
			HistoryTable[i][j].address=0;
		}
	}
	DBPT=malloc(sizeof(int)*65536);
	for(i=0; i<65536; i++){
		DBPT[i]=0;	
	}
}

void SWLTP::Predict(unsigned set, unsigned n_address, int* results){
	int* encodings=(int*)malloc(num_ways*sizeof(int));
	EncodeSet(set, n_address, encodings);
	int i=0;
	for(i=0; i<num_ways; i++){
		if((DBPT[encodings[i]]==2) || (DBPT[encodings[i]]==3)){
			results[i]=1; //predict dead
		} else {
			results[i]=0;		
		}
	}
	p_encodings=encodings;
}

void SWLTP::Feedback(unsigned way){ //
	int i=0;
	for(i=0; i<num_ways; i++){
		if(i==way){
			if(DBPT[p_encodings[i]]<=2){
				DBPT[p_encodings[i]]++;			
			}
		} else {
			if(DBPT[p_encodings[i]]>=1){
				DBPT[p_encodings[i]]--;			
			}
		}
	}

}

void SWLTP::EncodeSet(unsigned set, unsigned n_address, int* encodings){
	i=0;	
	for(i=0; i<num_ways; i++){
		encodings[i]=Encode(HistoryTable[set][i].mem_addr, n_address, HistoryTable[set][i].PC);
	}
}

int SWLTP::Encode(unsigned mem1, unsigned mem2, unsigned pc1){
	//needs to be changed, currently just XORs 
	unsigned buffer2=mem1^mem2;
	buffer2^=pc1;
	buffer2=buffer2>>16;
	return buffer2^buffer2;
}

void SWLTP::Update_History(unsigned set, unsigned way, unsigned mem_addr, unsigned pc_addr){
	HistoryTable[set][way].PC=pc_addr;
	HistoryTable[set][way].addr=mem_addr;
}




}  // namespace mem

