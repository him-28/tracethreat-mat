#include "CL/cl.h"

#define MAX_LENGTH 1024

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

