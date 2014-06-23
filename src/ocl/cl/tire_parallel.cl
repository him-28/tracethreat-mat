/*  Titles			                                                                  Authors	        Date
 * --Multi file array with multi signature file                                   R.Chatsiri
 */
int compare(__global const uchar * infected_str,
__local const uchar * symbol, 
uint length)
{
		for(uint count_length = 0; count_length < length; ++count_length)
		{
					if(infected_str[count_length] != symbol[count_length]) return 0;
				  
		}
		return  1;
}
/* Create Buffer */
__kernel void actire_search(
        __global uchar *node_symbol,
        __global int   *node_state,
        __global uchar *infected_str,
        __global uchar *symbol_wb,
        __global int * result_group_wb,
				__global int  * result_wb, 
        const int   binary_length,
        const int   symbol_length,
        const int   search_length_wg,
        __local  uchar* local_symbol)
{

			 __local volatile uint group_success_counter;

		   int local_idx  = get_local_id(0);
			 int local_size = get_local_size(0);
			 int group_idx  = get_group_id(0);


			 uint last_search_idx =  binary_length - search_length_wg + 1;

			 uint begin_search_idx = group_idx * search_length_wg;
			 uint end_search_idx   = begin_search_idx + search_length_wg;

	     if(begin_search_idx > last_search_idx) return;
			 if(end_search_idx > last_search_idx)  end_search_idx = last_search_idx;
	
			 for(int idx = local_idx; idx < symbol_length; idx+= local_size)
			 {
						local_symbol[idx] =  node_symbol[idx];
			 }

			 if(local_idx == 0) group_success_counter = 0;

			 barrier(CLK_LOCAL_MEM_FENCE);

			 for(uint string_pos = begin_search_idx + local_idx;
                string_pos < end_search_idx;
								string_pos += local_size)
			 {
					if(compare(infected_str + string_pos, local_symbol, symbol_length) == 1)
					{
					   int count = atomic_inc(&group_success_counter);
						  result_wb[begin_search_idx + count] = string_pos;
					}	

			 }

			 barrier(CLK_LOCAL_MEM_FENCE);
			  if(local_idx == 0)  result_group_wb[group_idx] = group_success_counter;
}
