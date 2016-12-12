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
#include <stdlib.h>
namespace mem
{

SWLTP::SWLTP(unsigned inum_sets, unsigned inum_ways){
	num_sets=inum_sets;
	num_ways=inum_ways;
	HistoryTable=(HistoryInfo**) new HistoryInfo*[num_sets];
	unsigned  i=0; unsigned j=0;
	for(i=0;i<num_sets; i++){
		HistoryTable[i]=(HistoryInfo*) new HistoryInfo[num_ways];	
		for (j = 0; j < num_ways; j++){
			HistoryTable[i][j].p_address =0;
			HistoryTable[i][j].p_encoding=-1;
			HistoryTable[i][j].c_address=0;
		}
	}

	DBPT=new int[65536];
	for(i=0; i<65536; i++){
		DBPT[i]=0;	
	}

        //initialize last touch count, misprediction count
        LastTouchCount = 0;
        MispredictCount = 0;
}


int SWLTP::Predict(unsigned set, unsigned way, unsigned pc){

        //performance evaluation metric - if the past encoding matches an entry in the table,
        // then we know that this block is being referenced again after a predicted last touch.
        // this is very bad! - Zach

        if(HistoryTable[set][way].ltPredicted)
        { 
                 //If the last touch for this block has been predicted already,
                 // then another access to the block would mean we have mispredicted a last touch
                 this->MispredictCount++;
                 HistoryTable[set][way].ltPredicted = false;
        }

	if(HistoryTable[set][way].p_encoding != -1)
        {

                DBPT[HistoryTable[set][way].p_encoding] = 0;
        }
	
	HistoryTable[set][way].p_encoding=Encode(HistoryTable[set][way].p_address, HistoryTable[set][way].c_address, pc);

        //On last touch, update the last touch count and set the prediction value to 'true'
        if(DBPT[HistoryTable[set][way].p_encoding] == 1)
        {
                this->LastTouchCount++;
                HistoryTable[set][way].ltPredicted = true;
        }
	return DBPT[HistoryTable[set][way].p_encoding];
}

void SWLTP::Feedback(unsigned set, unsigned way, unsigned n_address){ // on eviction, clear history table for this block
	DBPT[HistoryTable[set][way].p_encoding]=1;
	HistoryTable[set][way].p_encoding=-1;
	HistoryTable[set][way].p_address=HistoryTable[set][way].c_address;
	HistoryTable[set][way].c_address=n_address;	
        HistoryTable[set][way].ltPredicted = false;
}

int SWLTP::Encode(unsigned mem1, unsigned mem2, unsigned pc1){
	//needs to be changed, currently just XORs 
  unsigned buffer2=mem1^mem2; // current address and last address
	buffer2^=pc1;
        buffer2 >>= 4;
	buffer2&=65535; // takes last 16 bits

	return buffer2;
}




}  // namespace mem

