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
    };

    template<typename Buffer>
    class BufferSync
    {
        public:
						BufferSync(){ buff = new Buffer; buff->buffer_length = 0; }
            void set_buffer();
            volatile Buffer *buff;
        private:
    };

}


#endif /* CONTROLLER__BUFFER_SYNC_HPP_ */
