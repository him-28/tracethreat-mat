#ifndef CONTROLLER_CONDITION_VARIABLE_HPP
#define CONTROLLER_CONDITION_VARIABLE_HPP

#include "threadconcurrency/mutex_controller.hpp"

namespace controller
{

    class condition_variable
    {

        public:
            condition_variable();
            ~condition_variable();


            void signal();
            void broadcast();


            bool wait(mutex *in_mutex, int in_timeout_in_milsec);

            void wait(mutex *in_mutex);


        private:
            pthread_cond_t  m_hcondition;
            static void check_error(const char *context, int error);

    };

}




#endif
