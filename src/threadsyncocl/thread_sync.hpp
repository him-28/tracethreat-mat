#ifndef THREAD_SYNC_HPP
#define THREAD_SYNC_HPP
/*                       Titles                                          Authors                        Date
 *  - Sync controller between Thread and OCL                             R.Chatsiri                     08/10/2013
 *  - Get file to thread multithread                                     R.Chatsiri                     08/10/2013
 */

#include <iostream>
#include "threadsyncocl/thread_controller.hpp"
#include "utils/file_calculate.hpp"

//3rd
#include "boost/tuple/tuple.hpp"


namespace controller
{

    using namespace util;
    namespace h_util = hnmav_util;

    template<typename BufferSync>
    class ibuffer_sync
    {
        public:
            virtual boost::shared_ptr<BufferSync>& buffer_ocl() = 0;
            virtual std::string path()= 0;
            virtual ibuffer_sync& sync_processes() = 0;
    };

    template<typename BufferSync>
    class thread_sync : public ibuffer_sync<BufferSync>
    {

        private:
            boost::shared_ptr<BufferSync> buffer_sync;

            //file processes
            file_calculate<Extension> *file_cal;
            std::list<std::string> *list_files;

            //thread controller
            typedef BufferSync      buffer_sync_type;
            typedef buffer_kernel::size_int size_type;
            typedef boost::shared_ptr<comm_thread_buffer<buffer_sync_type> > thread_ptr;

            std::string  *file_path;
            thread_ptr   *thread_array_ptr;
            buffer_sync_type *buff_sync_internal;

            size_type thread_id;

            std::vector<thread_ptr> thread_ptr_vec;
            // pointer handler vector of shared_ptr of threads.
            std::vector<thread_ptr> *thread_pv_ptr;
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;


        public:

            thread_sync();

            boost::shared_ptr<BufferSync>& buffer_ocl();

            std::string path();
            void set_path(const char *path);

            //processe sync and controller
            std::vector<boost::shared_ptr<comm_thread_buffer<BufferSync> > >&   sync_init();
            bool sync_buffer();
            bool sync_check_kernel();

            boost::tuple<buffer_kernel::size_int> get_thread_info();

            ibuffer_sync<BufferSync>& sync_processes();

    };

}

#endif /* THREAD_THREAD_SYNC_HPP */
