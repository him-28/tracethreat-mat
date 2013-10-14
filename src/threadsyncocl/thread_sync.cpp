#include "threadsyncocl/thread_sync.hpp"

namespace controller
{


    template<typename BufferSync>
    thread_sync<BufferSync>::thread_sync()
    {
        thread_id = 0;
    }

    template<typename BufferSync>
    std::string thread_sync<BufferSync>::path()
    {
        return *file_path;
    }

    template<typename BufferSync>
    void thread_sync<BufferSync>::set_path(const char *path)
    {
        file_path = new std::string(path, strlen(path));
    }


    template<typename BufferSync>
    boost::shared_ptr<BufferSync>& thread_sync<BufferSync>::buffer_ocl()
    {
        return buffer_sync;
    }

    template<typename BufferSync>
    std::vector<boost::shared_ptr<comm_thread_buffer<BufferSync> > > & 
    thread_sync<BufferSync>::sync_init()
    {
        file_cal = new file_calculate<Extension>();
        std::string path_all = path();

        if(!file_cal->set_filepath(path_all.c_str())) {
            std::cout<<"File error " <<std::endl;
            return  thread_ptr_vec;
        }

        if(!file_cal->processes()) {
            std::cout<<" Process error " <<std::endl;
            return thread_ptr_vec;
        }

        int thread_size = 0;
        // thread per file.
        file_cal->file_size();
        list_files = &file_cal->get_files2buffer();
        thread_size = list_files->size();

        // Thread will start on thread_new
        for(typename std::list<std::string>::iterator
                iter_file =  list_files->begin();
                iter_file != list_files->end();
                ++iter_file, thread_id++) {

            thread_ptr_vec.push_back(
                    boost::shared_ptr<comm_thread_buffer<BufferSync> >(
                            new comm_thread_buffer<BufferSync>
                            (thread_id, &buff_sync_internal[thread_id]))
            );
        }

        return thread_ptr_vec;
    }


    template<typename BufferSync>
    boost::tuple<buffer_kernel::size_int> thread_sync<BufferSync>::get_thread_info()
    {
        return 	boost::tuple<buffer_kernel::size_int>(thread_ptr_vec.size());
    }

    template class controller::thread_sync<BufferSync<buffer_kernel> >;

}
