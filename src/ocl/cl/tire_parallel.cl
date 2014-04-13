/*  Titles			                                                                  Authors	        Date
 * --Multi file array with multi signature file                                   R.Chatsiri 
 */

/* Create Buffer */
__kernel void  actire_search(
__global uchar  *node_symbol,
__global uint  *node_state,
__global uchar *infected_str,
__global uchar *symbol_wb,
__global uint  *result)
{

	uint id_sym       = get_global_id(0);
	uint id_inf          = get_global_id(1);
	symbol_wb[id_sym] = infected_str[id_sym];
	uint sig_len = 10;

	if(node_symbol[0] == infected_str[id_sym])
	{
			 symbol_wb[id_sym] = 'v';
			for(int i = 1; i < sig_len; i++)
			{
						if(node_symbol[i] == infected_str[id_sym+i])
									symbol_wb[id_sym] = node_symbol[i];
			}

	}


}

