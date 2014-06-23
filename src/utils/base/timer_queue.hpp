#ifndef UTILS_INTERVAL_TIMER_QUEUE_HPP
#define UTILS_INTERVAL_TIMER_QUEUE_HPP

//timer
#include <boost/timer/timer.hpp>
#include <boost/chrono/chrono.hpp>
//#include <boost/detail/lightweight_main.hpp>

//multi_index
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/tag.hpp>
//standard
#include <cstdlib>
#include <iostream>
#include <locale>

namespace utils
{

    using boost::timer::nanosecond_type;
    using boost::timer::cpu_times;
    using boost::timer::cpu_timer;
    using boost::timer::auto_cpu_timer;
    using std::cout;
    using std::endl;

    struct time_result {
        int ds;
        int dms;
    };

    //using boost::multi_index;
    //base interval cpu time.
    class timer_queue
    {

        public:


            typedef int64_t Id;

            timer_queue::Id add(Id id, boost::timer::cpu_times start_times);

            timer_queue::Id add(Id id, double start_times);

            timer_queue::Id add(Id id, int start_time_sec, int start_time_usec);

            boost::timer::cpu_times get_start_timer();

            long int get_start_timer(Id id);

            double get_start_timer_linux(Id id);

            int get_stimer_basic_sec(Id id);

            int get_stimer_basic_usec(Id id);


            bool erase(Id id);

            long int interval_time(long int  start_time);

            double unix_time();

            double  interval_time(double current_time, double start_time);

            int  basic_time_sec();

            int  basic_time_usec();

            struct time_result interval_time(int start_time_sec, int start_time_usec);

        private:
            //timer
            boost::timer::cpu_timer cpu;
            boost::timer::cpu_times start_time;
            boost::timer::cpu_times current_time;

            struct event_time {
                Id  id;
                long int start_time;
                event_time(Id _id, long int  _start_time): id(_id),
                    start_time(_start_time) {

                }
            };

            typedef boost::multi_index_container<
            event_time,
            boost::multi_index::indexed_by<
            boost::multi_index::ordered_unique<
            ::boost::multi_index::member<
            event_time,
            Id, // id of time check
            &event_time::id
            >>,
            boost::multi_index::ordered_non_unique<
            ::boost::multi_index::member<
            event_time,
            long int,
            &event_time::start_time
            >>
            >
            > set_time;


            set_time set_internal_time;

            //event_time
            struct event_time_linux {
                Id  id;
                double start_time;
                event_time_linux(Id _id, double  _start_time): id(_id),
                    start_time(_start_time) {

                }
            };

            typedef boost::multi_index_container<
            event_time_linux,
            boost::multi_index::indexed_by<
            boost::multi_index::ordered_unique<
            ::boost::multi_index::member<
            event_time_linux,
            Id, // id of time check
            &event_time_linux::id
            >>,
            boost::multi_index::ordered_non_unique<
            ::boost::multi_index::member<
            event_time_linux,
            double,
            &event_time_linux::start_time
            >>
            >
            > set_time_linux;

            set_time_linux set_internal_linux_time;

            enum {
                BY_ID=0
            };
            //event_time
            struct event_time_basic {
                Id  id;
                int start_time_sec;
                int start_time_usec;

                event_time_basic(Id _id, int  _start_time_sec, int _start_time_usec): id(_id),
                    start_time_sec(_start_time_sec),
                    start_time_usec(_start_time_usec) {

                }
            };

            typedef boost::multi_index_container<
            event_time_basic,
            boost::multi_index::indexed_by<
            boost::multi_index::ordered_unique<
            ::boost::multi_index::member<
            event_time_basic,
            Id, // id of time check
            &event_time_basic::id
            >>,
            boost::multi_index::ordered_non_unique<
            ::boost::multi_index::member<
            event_time_basic,
            int,
            &event_time_basic::start_time_sec
            >>,
            boost::multi_index::ordered_non_unique<
            ::boost::multi_index::member<
            event_time_basic,
            int,
            &event_time_basic::start_time_usec
            >>
            >
            > set_time_basic;

            set_time_basic set_internal_basic;


            struct timeval   start_time_val;
            struct timeval   end_time;
    };





}



#endif /*UTILS_INTERVAL_TIMER_QUEUE_HPP */
