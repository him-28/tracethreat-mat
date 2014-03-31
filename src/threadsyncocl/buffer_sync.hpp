#ifndef CONTROLLER__BUFFER_SYNC_HPP_
#define CONTROLLER__BUFFER_SYNC_HPP_
// STL
#include  <iostream>

// 3rd
#include "CL/cl.h"

// internal
#include "utils/logger/clutil_logger.hpp"


#define MAX_LENGTH 1024

namespace controller
{
		namespace h_util = hnmav_util;
    //Declare forward which caller class.
    struct buffer_kernel;
    template<typename Buffer> class  BufferSync;

		struct ocldetail_scan{
			uint8_t status;
			uint64_t start_point; //start point of binary file.
			uint64_t end_point;   //end point of binary file.
				
		};

    struct buffer_kernel {
        cl_int  buffer_length;
        cl_char buffer_send;
        typedef int thread_int;
        typedef int size_int;
				//Support buffer block status
				std::vector<ocldetail_scan>
    };

    template<typename Buffer>
    class BufferSync
    {
        public:
            BufferSync(typename buffer_kernel::size_int  buffersync_size);
						BufferSync();
            bool set_buffer(controller::buffer_kernel::size_int, char * filename);
           volatile Buffer *buff;
           BufferSync<Buffer> & operator[](typename Buffer::size_int value)const;
           BufferSync<Buffer>& operator=(BufferSync<Buffer> * buffr);						
		       BufferSync<Buffer> & operator*(BufferSync<Buffer> & buffr);
					 
        private:
            typename Buffer::size_int size_buff;
					  BufferSync *buffersync_ptr;
						//logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };

			
}


#endif /* CONTROLLER__BUFFER_SYNC_HPP_ */
