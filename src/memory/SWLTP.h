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

namespace mem
{

class SWLTP
{
private:

	class HistoryInfo{
		public:
		unsigned PC;
		unsigned address;

	}

	unsigned num_sets, num_ways;
	HistoryInfo** HistoryTable;
	int* DBPT;	
	int* p_encodings;

	


public:

        
	//Constructor
        SWLTP::SWLTP(unsigned inum_sets, unsigned inum_ways);
	
	//Given a set and a new address belonging to that set, return an array of predictions for every way
	void SWLTP::Predict(unsigned set, unsigned n_address, int* results);

	//For the saturating counter of DBPT, increment or decrement from the array p_encodings
	void SWLTP::Feedback(unsigned way);

	//Encode everything in a set into encodings
	void SWLTP::EncodeSet(unsigned set, unsigned n_address, int* encodings);

	//Encode a single 1PC2SAddrF
	int SWLTP::Encode(unsigned mem1, unsigned mem2, unsigned pc1);

	//Every time a block is accessed update the memory address and pc address. The constant memory address update may be redundant, however this is the easiest way to ensure SWLTP has the latest address in the history table
	void SWLTP::Update_History(unsigned set, unsigned way, unsigned mem_addr, unsigned pc_addr);
};

}


