/*  Titles			                                                                  Authors	        Date
 * --Multi file array with multi signature file                                   R.Chatsiri
 */

/* Create Buffer */
__kernel void actire_search(
        __global uchar *node_symbol,
        __global int   *node_state,
        __global uchar *infected_str,
        __global uchar *symbol_wb,
        __global uint *result,
				__global int  *state_wb)
{

    uint id_sym0 = get_global_id(0);

  	uint state = 0;


    if(node_symbol[0] == infected_str[id_sym0]) {

        uint count_state = 1;

				uint count_state_wb = 1;

        symbol_wb[id_sym0] = node_symbol[state];

        for(uint count_inf = id_sym0 + 1; count_inf < id_sym0+24; count_inf++) {

			     if(node_symbol[node_state[count_state]] != infected_str[count_inf]) { // count_state

               count_state = 1;
							break;		
            }

						state_wb[count_inf]  =  node_state[count_state];					
	
            count_state++;
						count_state_wb++;
        }//for
    }//if

}


/*
__kernel void actire_search(
        __global uchar *node_symbol,
        __global int *node_state,
        __global uchar *infected_str,
        __global uchar *symbol_wb,
        __global uint *result)
{

uint id_sym0 = get_global_id(0);

uint state = 0;

if(node_symbol[state] == infected_str[id_sym0])
{
	uint count_state = 1;
	uint count_index = 0;

	for(uint count_inf = id_sym0 + 1; count_inf < id_sym0 + 31; count_inf++){
			symbol_wb[count_index] = 'x';//infected_str[count_inf];
			count_index++;
	}//for
}//if



}//end

*/


/*
__kernel void actire_search(
__global uchar *node_symbol,
__global int *node_state,
__global uchar *infected_str,
__global uchar *symbol_wb,
__global uint *result)
{

uint id_sym0 = get_global_id(0);

uint state = 0;

if(node_symbol[state] == infected_str[id_sym0+128])
{
	uint count_state = 1;
	uint count_index = 0;
	//symbol_wb[id_sym0] = node_symbol[state];

	for(uint count_inf = id_sym0 + 129; count_inf < 160; count_inf++){
			symbol_wb[count_index] = infected_str[count_inf];
			count_index++;
	}//for
}//if

}//end

*/
