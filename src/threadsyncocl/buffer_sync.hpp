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
            BufferSync(typename buffer_kernel::size_int  buffersync_size) {
                
	    					size_buff = buffersync_size > 0 ? buffersync_size : 1;
								 	
								buffersync_ptr = new BufferSync<Buffer>[size_buff];
								for(int count_buff = 0; count_buff < size_buff; count_buff++)
								{
										buffersync_ptr =  new BufferSync<Buffer>(0); //default size 
								}
					                
            }

						BufferSync(){

								buff = new Buffer;
                buff->buffer_length = 0;
	

						}

            void set_buffer();
            volatile Buffer *buff;

            BufferSync<Buffer> & operator[](typename Buffer::size_int value)const {
                if(value < 0 || value >= size_buff)
                     std::cout<<"Size error " <<std::endl; // todo : error try catch 
                return buffersync_ptr[value];
            }

						

            BufferSync<Buffer>& operator=(BufferSync<Buffer>& buffr) {
                if(&buffr != this) {
                    if(size_buff != buffr.size_buff) {

                        delete [] buffersync_ptr;
                        size_buff = buffr.size_buff;
                        buffersync_ptr = new BufferSync[size_buff];
                        //return buffersync_ptr;
                    }
                }

                for(typename Buffer::size_int size_count =  0; size_count < size_buff; size_count++) {
                    buffersync_ptr[size_count] = buffr[size_count];
                }

                return *this;
            }
					
						

        private:
            typename Buffer::size_int size_buff;
            BufferSync *buffersync_ptr;
    };

			
}


#endif /* CONTROLLER__BUFFER_SYNC_HPP_ */
