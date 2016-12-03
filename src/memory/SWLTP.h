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
		int p_encoding;
		unsigned c_address;
		unsigned p_address;

	};

	unsigned num_sets, num_ways;
	HistoryInfo** HistoryTable;
	int* DBPT;	

	


public:

        
	//Constructor
        SWLTP(unsigned inum_sets, unsigned inum_ways);
	
	//Destructor
	~SWLTP(unsigned inum_sets, unsigned inum_ways);
	
	//Given a set and a new address belonging to that set, return an array of predictions for every way
	int Predict(unsigned set, unsigned way, unsigned pc);

	//For the saturating counter of DBPT, increment or decrement from the array p_encodings
	void Feedback(unsigned set, unsigned way, unsigned n_addr);

	//Encode a single 1PC2SAddrF
	int Encode(unsigned mem1, unsigned mem2, unsigned pc1);

};

}


