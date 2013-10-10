#include "threadsyncocl/thread_sync.hpp"

namespace controller
{


    template<typename BufferSync>
    thread_sync<BufferSync>::thread_sync()
    {

    }

    template<typename BufferSync>
    std::string thread_sync<BufferSync>::path()
    {
        return "";
    }

    template<typename BufferSync>
    boost::shared_ptr<BufferSync>& thread_sync<BufferSync>::buffer_ocl()
    {
        return buffer_sync;
    }

    template<typename BufferSync>
    bool thread_sync<BufferSync>::sync_init()
    {
        file_cal = new file_calculate<Extension>();
        std::string path_all = path();

        if(file_cal->set_filepath(path_all.c_str()))
            return false;

        if(file_cal->processes())
            return false;

        // thread per file.
        list_files = &file_cal->get_files2buffer();
        int thread_size = list_files->size();
        typename buffer_kernel::size_int thread_id = 0;
        buff_sync_internal = new BufferSync(thread_size);

        for(typename std::list<std::string>::iterator
                iter_file =  list_files->begin();
                iter_file != list_files->end();
                ++iter_file, thread_id++) {

            //thread_array_ptr[thread_id] =
            boost::shared_ptr<comm_thread_buffer<BufferSync> >
            thread(new comm_thread_buffer<BufferSync>(thread_id,
                    &buff_sync_internal[thread_id]) );
            thread_ptr_vec.push_back(thread);

        }

        return true;
    }

    template class controller::thread_sync<BufferSync<buffer_kernel> >;
    template class controller::thread_sync<int>;

}
