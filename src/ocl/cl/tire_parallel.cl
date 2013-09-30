/*                       Titles                                          Authors                        Date
*-Add struct and create function vector add string from virus.db        Chatsiri.rat                   27/11/2012
*
*/

#include <struct_tire.h>


/* Create Buffer */
__kernel void  create_tire_buffer(
__global node_data *node_input,
__global char * sigrc_input,
 int node_input_sizes, 
__global uint *  pw_dim, 
__global uint  * pg_id,
__global char * sigrc)
{

    uint work_dim = get_work_dim();
    uint w_dim = work_dim - 1;
	  
	  int lid = get_local_id(0);
		int gb_size = get_local_size(0);

		int idx = get_global_id(0);

		
		 sigrc[idx] = sigrc_input[idx];
}

/*
   size_t  local_size = get_local_size(w_dim);
   printf(" Local size  : %d \n", local_size);

   size_t num_groups = get_num_groups(0);
   printf(" Number Groups : %d \n", num_groups);

   size_t group_id   = get_group_id(0);
   printf(" Group ID      : %d \n", group_id);

   size_t global_offset = get_global_offset(0);
   printf(" Global offset : %d \n", global_offset);

   printf("Data in gropu id : %c \n", input[global_id]);
*/

