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
			HistoryTable[i][j].p_address =0;
			HistoryTable[i][j].p_encoding=0;
		}
	}
	DBPT=malloc(sizeof(int)*65536);
	for(i=0; i<65536; i++){
		DBPT[i]=0;	
	}
}

int SWLTP::Predict(unsigned set, unsigned way, unsigned address, unsigned pc){
	HistoryTable[set][way].p_encoding=Encode(HistoryTable[set][way].p_address, address, pc);
	return DBPT[HistoryTable[set][way].p_encoding];
}

void SWLTP::Feedback(unsigned set, unsigned way, unsigned address){ //
	DBPT[HistoryTable[set][way].p_encoding]=1;
	HistoryTable[set][way].p_address=address;	
}

int SWLTP::Encode(unsigned mem1, unsigned mem2, unsigned pc1){
	//needs to be changed, currently just XORs 
	unsigned buffer2=mem1^mem2;
	buffer2^=pc1;
	buffer2=buffer2>>16;
	return buffer2^buffer2;
}




}  // namespace mem

