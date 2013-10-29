#ifndef DATA_STRUCTURE__AC_TIRE_HPP
#define DATA_STRUCTURE__AC_TIRE_HPP

/*                       Titles                                          Authors                        Date
 *- array create work items & work group                                 Chatsiri.rat								  08/02/2013
 */

#define AC_CHAR 256

#include <vector>

namespace data_structure
{

struct NodeStruct
{
	typedef char char_t;
	typedef unsigned int  int_t;
};

template<typename NodeStruct>
struct node_state
{
	typename NodeStruct::int_t id;
	typename NodeStruct::int_t depth;
	typename NodeStruct::char_t * output;
	struct node_state<NodeStruct> * fail;
	std::vector<node_state<NodeStruct> > next;
	node_state(){ output = 0; } 
};

template<typename NodeStruct>
struct node
{
	typename NodeStruct::int_t idcounter;
	typename NodeStruct::int_t pattern_counter;
	struct node_state<NodeStruct> * zero_state; 
	node(){ zero_state = 0; }
};

template<typename NodeStruct>
class ac_tire
{

public:
	void ac_init(struct node<NodeStruct> * node_table);
	int  ac_addstring(struct node<NodeStruct> * node_table,const typename NodeStruct::char_t * str, int slen);
	int  ac_maketree(struct node<NodeStruct> * node_table);	
private:
	std::vector<node<NodeStruct> > node_table_;
	std::vector<node_state<NodeStruct> > node_state_;
};

}



#endif
