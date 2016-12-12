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


int SWLTP::Predict(unsigned set, unsigned way, unsigned pc, unsigned n_addr)
{

        if(HistoryTable[set][way].ltPredicted)
        { 
                 //If the last touch for this block has been predicted already,
                 // then another access to the block would mean we have mispredicted a last touch
                 this->MispredictCount++;
                 HistoryTable[set][way].ltPredicted = false;
        }

	if(HistoryTable[set][way].p_encoding != -1)
        {
        
                //bad signature (or aliasing...) set counter to 0
                DBPT[HistoryTable[set][way].p_encoding] = 0;
        }

        //update history table - why weren't we doing this before?
        HistoryTable[set][way].p_address = HistoryTable[set][way].c_address;
        HistoryTable[set][way].c_address = n_addr;
	
        //Encode new trace
	HistoryTable[set][way].p_encoding = 
                Encode(HistoryTable[set][way].p_address, HistoryTable[set][way].c_address, pc);

        //On last touch, update the last touch count and set the prediction value to 'true'
        // If saturating counter is 2 or 3 for this signature, predict last touch
        if(DBPT[HistoryTable[set][way].p_encoding] > 1)
        {
                this->LastTouchCount++;
                HistoryTable[set][way].ltPredicted = true;
                return 1;
        }

        //if it is not, don't predict last touch
        
        //return '1' if last touch, '0' if not
	return 0;
}

void SWLTP::Feedback(unsigned set, unsigned way)
{       // on eviction, clear history table for this block
	DBPT[HistoryTable[set][way].p_encoding] = 3 ? 3 : DBPT[HistoryTable[set][way].p_encoding] + 1;

        //on eviction, clear history table for this block
	HistoryTable[set][way].p_encoding = -1;
	HistoryTable[set][way].p_address = 0;
	HistoryTable[set][way].c_address = 0;	
        HistoryTable[set][way].ltPredicted = false;
}

int SWLTP::Encode(unsigned mem1, unsigned mem2, unsigned pc1)
{
	//XOR style hashing
        unsigned buffer2=mem1^mem2; // current address and last address
	buffer2^=pc1;
        buffer2 >>= 4;
	buffer2&=65535; // takes last 16 bits

	return buffer2;
}




}  // namespace mem

