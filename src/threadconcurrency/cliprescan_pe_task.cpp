#include "threadconcurrency/cliprescan_pe_task.hpp"
#include "exception/system_exception.hpp"

namespace controller
{

    cliprescan_pe_task::cliprescan_pe_task(monitor_controller& monitor, size_t& count, int64_t timeout) :
        _monitor(monitor),
        _count(count),
        _timeout(timeout),
        _done(false) {}

    void cliprescan_pe_task::run()
    {


        _startTime = util_thread::current_time();

        {
            synchronized s(_sleep);

            try {
                _sleep.wait(_timeout);
            } catch(exceptions::timed_out_exception& e) {
                std::cout<<" Thread exceptions::timed_out_exception& " <<std::endl;
            } catch(...) {
                assert(0);
            }
        }

        _endTime =  util_thread::current_time();

        _done = true;

        {
            synchronized s(_monitor);

            std::cout << "Thread " << _count << " completed " << std::endl;

            _count--;

            if (_count == 0) {

                _monitor.notify();
            }
        }


    }


}
