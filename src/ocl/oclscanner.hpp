#ifndef KERNEL_OCLSCANNER_HPP
#define KERNEL_OCLSCANNER_HPP

namespace kernel
{

    class oclscanner
    {
            virtual ~oclscan();
            virtual bool load_default_system() = 0;
            virtual bool send_buffer() = 0;
            virtual bool pre_filter_binary() = 0;
            virtual bool scan() = 0;
    };


}


#endif
