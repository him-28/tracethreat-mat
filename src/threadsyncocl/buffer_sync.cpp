#include "buffer_sync.hpp"

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
				std::cout<<" New init , Size buffer set : " << this->size_buff <<std::endl;
    }


    template<typename Buffer>
    void BufferSync<Buffer>::set_buffer()
    {
			  this->size_buff = 0;
				std::cout<<" Test " <<std::endl;	
    }

    template<typename Buffer>
    BufferSync<Buffer>& BufferSync<Buffer>::operator[](typename Buffer::size_int value)const
    {
        if(value < 0 || value >= size_buff)
            std::cout<<"Size error " <<std::endl; // todo : error try catch

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
