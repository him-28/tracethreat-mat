#include "utils/base/timer_queue.hpp"
#include <sys/time.h>

namespace utils
{
		//___________________________ Kernel Time interval _______________________
    timer_queue::Id timer_queue::add(Id id,boost::timer::cpu_times start_times)
    {
        set_internal_time.get<1>().insert(event_time(id, start_time.wall));
        //struct event_time  et
        struct event_time et  = *set_internal_time.get<0>().find(id);
        return et.id;
    }

    long int timer_queue::get_start_timer(Id id)
    {
        struct event_time et  = *set_internal_time.get<0>().find(id);
        return et.start_time;
    }


    boost::timer::cpu_times timer_queue::get_start_timer()
    {
        if(boost::chrono::high_resolution_clock::is_steady)
            std::cout<<"Steady" <<std::endl;

        cpu.start();
        start_time.clear();
        start_time.wall = cpu.elapsed().wall;
        current_time.wall = start_time.wall;
        return current_time;
    }

    bool timer_queue::erase(Id id)
    {
        return set_internal_basic.get<BY_ID>().erase(id);
    }

    long int timer_queue::interval_time(long int  st)
    {
        long int st_temp;

        while(current_time.user == st) {
            current_time.wall = cpu.elapsed().wall;
        }

        st_temp = current_time.wall - st;
        return st_temp;
    }

    //______________________________  Map basic time interval _________________________________
    double timer_queue::get_start_timer_linux(Id id)
    {
        struct event_time_linux et_linux  = *set_internal_linux_time.get<0>().find(id);
        return et_linux.start_time;
    }

    double  timer_queue::interval_time(double current_time, double start_time)
    {

        if(current_time == start_time ||
                current_time < start_time)
            return double(0);

        double result = current_time - start_time;
        return result;
    }
    double timer_queue::unix_time()
    {
        struct timespec tv;

        if(clock_gettime(CLOCK_REALTIME, &tv) != 0) return 0;

        return (((double) tv.tv_sec) + (double)(tv.tv_nsec / 1.0e9));
    }

    timer_queue::Id timer_queue::add(Id id,double start_times)
    {
        set_internal_linux_time.get<1>().insert(event_time_linux(id, start_times));
        //struct event_time  et
        struct event_time_linux et_linux  = *set_internal_linux_time.get<0>().find(id);
        return et_linux.id;
    }

    int  timer_queue::basic_time_sec()
    {
        memset(&start_time_val, 0, sizeof(timeval));
        gettimeofday(&start_time_val, 0);
        return start_time_val.tv_sec;
    }

    int  timer_queue::basic_time_usec()
    {
        return start_time_val.tv_sec;
    }

    struct	time_result  timer_queue::interval_time(int start_time_sec, int start_time_usec) {
        struct time_result time_ret;
        gettimeofday(&end_time, 0);
        int ds = end_time.tv_sec - start_time_sec;
        int dms = end_time.tv_usec - start_time_usec;
        ds -= (dms < 0) ? (1) :(0);
        dms += (dms < 0) ? (1000000) : (0);
        time_ret.ds = ds;
        time_ret.dms = dms;
        return time_ret;
    }

    //______________________________________ Basic Time interval _________________________________________
    timer_queue::Id timer_queue::add(Id id,int start_time_sec, int start_time_usec)
    {
        set_internal_basic.get<1>().insert(event_time_basic(id, start_time_sec, start_time_usec));
        //struct event_time  et
        struct event_time_basic et_basic  = *set_internal_basic.get<0>().find(id);

        return et_basic.id;
    }

    int timer_queue::get_stimer_basic_sec(Id id)
    {
        struct event_time_basic et_basic  = *set_internal_basic.get<0>().find(id);
        return et_basic.start_time_sec;
    }

    int timer_queue::get_stimer_basic_usec(Id id)
    {
        struct event_time_basic et_basic  = *set_internal_basic.get<0>().find(id);
        return et_basic.start_time_usec;
    }

    int timer_queue::timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
    {
        long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
        result->tv_sec = diff / 1000000;
        result->tv_usec = diff % 1000000;

        return (diff<0);
    }

    void timer_queue::timeval_print(struct timeval *tv)
    {
        char buffer[30];
        time_t curtime;

        printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
        curtime = tv->tv_sec;
        strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
        printf(" = %s.%06ld\n", buffer, tv->tv_usec);
    }

}


