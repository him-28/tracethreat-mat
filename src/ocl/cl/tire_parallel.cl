/*  Titles			                                                                  Authors	        Date
 *
 */

/* Create Buffer */
__kernel void  actire_search(
__global uchar  *node_symbol,
__global uint  *node_state,
__global uchar *infected_str,
__global uchar *symbol_wb)
{
	uint count_global_id = get_global_id(0);
	symbol_wb[count_global_id] = node_symbol[count_global_id];		
}

