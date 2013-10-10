#ifndef THREAD_SYNC_HPP
#define THREAD_SYNC_HPP
/*                       Titles                                          Authors                        Date
 *  - Sync controller between Thread and OCL                             R.Chatsiri                     08/10/2013
 *  - Get file to thread multithread                                     R.Chatsiri                     08/10/2013
 */

#include <iostream>
#include "threadsyncocl/thread_controller.hpp"
#include "utils/file_calculate.hpp"

namespace controller
{

    using namespace util;

    template<typename BufferSync>
    class ibuffer_sync
    {
        public:
            virtual boost::shared_ptr<BufferSync>& buffer_ocl() = 0;
            virtual std::string path() = 0;
    };

    template<typename BufferSync>
    class thread_sync : public ibuffer_sync<BufferSync>
    {
        public:

            thread_sync();

            boost::shared_ptr<BufferSync>& buffer_ocl();

            std::string path();

            //processe sync and controller
            bool sync_init();
            bool sync_buffer();
            bool sync_check_kernel();

            // for test case only
            //void pre_check_strucS();

        private:
            //thread_controller<BufferSync> thread_cont;
            boost::shared_ptr<BufferSync> buffer_sync;

            //file processes
            file_calculate<Extension> *file_cal;
            std::list<std::string> *list_files;

            //thread controller
            typedef BufferSync      buffer_sync_type;
            typedef buffer_kernel::size_int size_type;
            typedef boost::shared_ptr<comm_thread_buffer<buffer_sync_type> > thread_ptr;

            thread_ptr   *thread_array_ptr;
            buffer_sync_type *buff_sync_internal;

            std::vector<thread_ptr> thread_ptr_vec;

    };

}

#endif /* THREAD_THREAD_SYNC_HPP */
