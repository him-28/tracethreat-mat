
#include "threadconcurrency/condition_variable.hpp"

#include <stdexcept>
#include <string>
#include <sys/time.h>
#include <assert.h>

namespace controller
{

    condition_variable::condition_variable()
    {
        pthread_condattr_t cond_attr;
        pthread_condattr_init(&cond_attr);
        int ret = pthread_cond_init(&m_hcondition, &cond_attr);
        pthread_condattr_destroy(&cond_attr);
        check_error("condition_variable::condition_variable", ret);
    }

    condition_variable::~condition_variable()
    {
        pthread_cond_destroy(&m_hcondition);
    }

    void condition_variable::check_error(const char *context, int error)
    {

        if(error != 0) {
            std::string msg = context;
            msg += "error : ";
            msg += std::strerror(error);
            throw std::runtime_error(msg);
        }

    }


    void condition_variable::broadcast()
    {
        check_error("condition_variable::boradcast()", pthread_cond_braodcast(&m_hcondition));
    }

    void condition_variable::signal()
    {

        check_error("condition_variable::signal()", pthread_cond_signal(&m_hcondition));

    }

    void condition_variable::wait(mutex *in_mutex)
    {
        check_error("condition_variable::wait", pthread_cond_wait(&m_hcondition,
           &in_mutex->get_underlying_impl()));
    }

    bool condition_variable::wait(mutex *in_mutex, int in_timeout_in_milsec)
    {
        if(in_timeout_in_milsec <0) {
            wait(in_mutex);
            return true;
        }

        //get time current
        struct timeval tv;
        gettimeofday(&tv, NULL);

        tv.tv_sec += in_timeout_in_milsec / 1000;
        tv.tv_usec += (in_timeout_in_milsec % 1000) * 1000;

        int million = 1000000;

        if(tv.tv_usec >= million) {
            tv.tv_sec += tv.tv_usec / million;
            tv.tv_usec %= million;
        }

        // convert time interval to time space.
        struct timespec tss;
        ts.tv_sec = tv.tv_sec;
        ts.tv_nsec = tv.tv_usec * 1000;
        int error = pthread_cond_timedwait(&m_hcondition, &in_mutex->get_underlying_impl(), & ts);

        if(error == ETIMEDOUT)
            return false;
        else
            check_error("condition_variable::wait", error);

        return true;

    }

}

