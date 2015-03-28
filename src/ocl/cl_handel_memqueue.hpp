/*                       Titles                                          Authors                        Date
 *-Class handeling memory function on system                             Chatsiri.rat                   03/12/2012
 *  -- Class implements with base_clutil
 */
#ifndef KERNEL_HANDEL_CL_HANDEL_MEMQUEUE_HPP
#define KERNEL_HANDEL_CL_HANDEL_MEMQUEUE_HPP


namespace kernel
{

    namespace handel
    {

        template<typename BaseCLUtil>
        class cl_handel : public base_clutil
        {
            public:
                BaseCLUtil *process();
                virtual BaseCLUtil *handel_read();
                virtual bool handel_write();
            private:
                BaseCLUtil *handel;
        };

        template<typename BaseCLUtil>
        BaseCLUtil *cl_handel<BaseCLUtil>::process()
        {
            handel = handel_read();
            return &handel;
        }

        template<typename BaseCLUtil>
        class cl_handel_memory : public cl_handel<BaseCLUtil>
        {
            public:
                BaseCLUtil *handel_read();
                bool handel_write();
            private:
                BaseCLUtil *handel_memory;
        };


        template<typename BaseCLUtil>
        BaseCLUtil cl_handel_memory<BaseCLUtil>::handel_read()
        {

        }

        template<typename BaseCLUtil>
        BaseCLUtil cl_handel_memory<BaseCLUtil>::handel_read()
        {

        }



    }
}

#endif /* HNMAV_KERNEL_HANDEL_CL_HANDEL_MEMQUEUE_HPP */
