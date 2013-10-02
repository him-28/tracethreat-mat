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

    //volatile BufferSync<buffer_kernel> buff_sync_volatile;

    struct buffer_kernel {
        cl_int  buffer_length;
    };

    template<typename Buffer>
    class BufferSync
    {
        public:
            void set_buffer();
        private:
            volatile Buffer *buff;
    };

}


#endif /* CONTROLLER__BUFFER_SYNC_HPP_ */
