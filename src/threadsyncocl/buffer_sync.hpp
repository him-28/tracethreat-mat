#ifndef CONTROLLER__BUFFER_SYNC_HPP_
#define CONTROLLER__BUFFER_SYNC_HPP_
// STL
#include  <iostream>

// 3rd
#include "CL/cl.h"
//#include "pthread.h"

#define MAX_LENGTH 1024

namespace controller
{
    //Declare forward which caller class.
    struct buffer_kernel;
    template<typename Buffer> class  BufferSync;

    struct buffer_kernel {
        cl_int  buffer_length;
        cl_char buffer_send;
        typedef int thread_int;
        typedef int size_int;
    };

    template<typename Buffer>
    class BufferSync
    {
        public:
            BufferSync(typename buffer_kernel::size_int  buffersync_size);
						BufferSync();
           void set_buffer();
           volatile Buffer *buff;
           BufferSync<Buffer> & operator[](typename Buffer::size_int value)const;
           BufferSync<Buffer>& operator=(BufferSync<Buffer> * buffr);						
		       BufferSync<Buffer> & operator*(BufferSync<Buffer> & buffr);

        private:
            typename Buffer::size_int size_buff;
					  BufferSync *buffersync_ptr;

    };

			
}


#endif /* CONTROLLER__BUFFER_SYNC_HPP_ */
