#include "CL/cl.h"
#ifndef CONTROLLER__BUFFER_SYNC_HPP_
#define CONTROLLER__BUFFER_SYNC_HPP_

#define MAX_LENGTH 1024
namespace controller
{

struct buffer_kernel
{
	cl_int  buffer_length[MAX_LENGTH];
};

template<typename Buffer>
class BufferSync
{
	public:
		void set_buffer();	
	private:
	Buffer * buff;
};

}

#endif /* CONTROLLER__BUFFER_SYNC_HPP_ */
