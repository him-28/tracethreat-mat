#ifndef KERNEL_OCLSCANNER_HPP
#define KERNEL_OCLSCANNER_HPP

namespace kernel
{

    class oclscanner
    {
            virtual ~oclscan();
            virtual load_default_system() = 0;
            virtual send_buffer() = 0;
            virtual pre_filter_binary() = 0;
            virtual scan() = 0;
    };


}


#endif
