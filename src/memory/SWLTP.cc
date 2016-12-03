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
	HistoryTable=(HistoryInfo**) new HistoryInfo*[num_sets];
	unsigned  i=0; unsigned j=0;
	for(i=0;i<num_ways; i++){
		HistoryTable[i]=(HistoryInfo*) new HistoryInfo[num_ways];	
	}

	for (i = 0; i <  num_sets; i++){
		for (j = 0; j < num_ways; j++){
			HistoryTable[i][j].p_address =0;
			HistoryTable[i][j].p_encoding=0;
		}
	}
	DBPT=new int[65536];
	for(i=0; i<65536; i++){
		DBPT[i]=0;	
	}
}
	
	SWLTP::~SWLTP(unsigned inum_sets, unsigned inum_ways){
	num_sets=inum_sets;
	num_ways=inum_ways;
	HistoryTable=(HistoryInfo**) delete HistoryInfo*[num_sets];
	unsigned  i=0; unsigned j=0;
	for(i=0;i<num_ways; i++){
		HistoryTable[i]=(HistoryInfo*) delete HistoryInfo[num_ways];	
	}

	DBPT= delete int[65536];
}


int SWLTP::Predict(unsigned set, unsigned way, unsigned pc){
	if(HistoryTable[set][way].p_encoding!=0){
		DBPT[HistoryTable[set][way].p_encoding]=0;	
	}
	
	HistoryTable[set][way].p_encoding=Encode(HistoryTable[set][way].p_address, HistoryTable[set][way].c_address, pc);
	return DBPT[HistoryTable[set][way].p_encoding];
}

void SWLTP::Feedback(unsigned set, unsigned way, unsigned n_address){ //
	DBPT[HistoryTable[set][way].p_encoding]=1;
	HistoryTable[set][way].p_encoding=0;
	HistoryTable[set][way].p_address=HistoryTable[set][way].c_address;
	HistoryTable[set][way].c_address=n_address;	
}

int SWLTP::Encode(unsigned mem1, unsigned mem2, unsigned pc1){
	//needs to be changed, currently just XORs 
	unsigned buffer2=mem1^mem2;
	buffer2^=pc1;
	buffer2=buffer2>>16;
	return buffer2^buffer2;
}




}  // namespace mem

