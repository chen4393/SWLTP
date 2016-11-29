/*
*
* M2S SRRIP With LTP Implementation
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
        

 public:

        //placeholder functions
        void predictBlock(unsigned ref_addr, unsigned PC_addr);
        void updateLTTable(unsigned set_id, unsigned way_id);

        SWLTP()
        {
        }

        
};

}


