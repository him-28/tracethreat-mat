/*  Titles			                                                                  Authors	        Date
 * --Multi file array with multi signature file                                   R.Chatsiri 
 */

/* Create Buffer */
__kernel void  actire_search(
__global uchar *node_symbol,
__global uint  *node_state,
__global uchar *infected_str,
__global uchar *symbol_wb,
__global uint  *result)
{

	uint id_sym0 = get_global_id(0);
	//uint id_sym1 = get_global_id(1);
	//symbol_wb[id_sym0] = infected_str[id_sym0];
						
	if(infected_str[id_sym0] == node_symbol[0])
	{
		int count_symbol = 1;
		for(int j = id_sym0+1; j < id_sym0+10; j++)
		{
			if(infected_str[j] != node_symbol[count_symbol])
			{
				break;
			}else{
					symbol_wb[j] = 'x';// infected_str[j];
			}
			count_symbol++;
		}
	}

}

