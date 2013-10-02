#include "buffer_sync.hpp"

namespace controller
{
template<typename Buffer>
void BufferSync<Buffer>::set_buffer()
{
	  buff->buffer_length++;
		std::cout<<" Set buffer : " <<buff->buffer_length <<std::endl;
}

template class BufferSync<buffer_kernel>;
}
