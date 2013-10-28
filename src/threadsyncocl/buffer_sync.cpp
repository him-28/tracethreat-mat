#include "buffer_sync.hpp"
#define MAX_BUFFER_SIZE 500000

namespace controller
{
    template<typename Buffer>
    BufferSync<Buffer>::BufferSync(typename buffer_kernel::size_int  buffersync_size)
    {

        size_buff = buffersync_size > 0 ? buffersync_size : 1;
        buffersync_ptr = new BufferSync<Buffer>[size_buff];
				for(int count_buff = 0; count_buff < size_buff; count_buff++)
				{
						buffersync_ptr[count_buff] = new BufferSync<Buffer>();
				}
    }

    template<typename Buffer>
    BufferSync<Buffer>::BufferSync()
    {
				this->size_buff = 0;
				this->buff = new Buffer;
				this->buff->buffer_length = 0;
				// logger 
 				logger_ptr = &h_util::clutil_logging<std::string, int>:: get_instance();
				logger = logger_ptr->get();
				logger->write_info("BufferSync, init size of size_buff");
    }


    template<typename Buffer>
    bool BufferSync<Buffer>::set_buffer(typename buffer_kernel::size_int buffer, char * file_name)
    {
				if(this->buff->buffer_length >= MAX_BUFFER_SIZE)
				 	return false;

				this->buff->buffer_length++;
/*				
				std::ifstream read_binary(file_name, std::ios::in | std::ios::binary);
				read_binary.seekg(0, std::ios::end);
				int file_length_size = read_binary.tellg();
				read_binary.seekg(0, std::ios::beg);
*/
				return true;
    }

    template<typename Buffer>
    BufferSync<Buffer>& BufferSync<Buffer>::operator[](typename Buffer::size_int value)const
    {
        if(value < 0 || value >= size_buff)
            logger->write_info("BufferSync, Size error ");// todo : error try catch

        return buffersync_ptr[value];
    }

    template<typename Buffer>
    BufferSync<Buffer>& BufferSync<Buffer>::operator=(BufferSync<Buffer> * buffr)
    {
					
        if(buffr != this) {
            if(size_buff != buffr->size_buff) {
                delete [] buffersync_ptr;
                size_buff = buffr->size_buff;
                buffersync_ptr = new BufferSync[size_buff];
            }
        }

        for(typename Buffer::size_int size_count =  0; size_count < size_buff; size_count++) {
            buffersync_ptr[size_count] = buffr[size_count];
        }
        return *this;
    }

		template<typename Buffer>
		BufferSync<Buffer> & BufferSync<Buffer>::operator*(BufferSync<Buffer> & buffr)
		{
				//todo: 
		}

    template class BufferSync<buffer_kernel>;

}
