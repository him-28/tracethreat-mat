
#include "exception/system_exception.hpp"
#include "threadconcurrency/cliprescan_pe_task.hpp"

namespace controller
{
    using namespace wrapper;

    cliprescan_pe_task::cliprescan_pe_task(monitor_controller& monitor, 
				size_t& count, 
				int64_t timeout) :
        _monitor(monitor),
        _count(count),
        _timeout(timeout),
        _done(false) {}

    bool cliprescan_pe_task::set_file_scanpath(const char *file_scanpath)
    {
        this->file_scanpath = file_scanpath;
        return true;
    }

    bool cliprescan_pe_task::set_file_sigdb(const char *file_sigdb)
    {
        this->file_sigdb = file_sigdb;
        return true;
    }

    void cliprescan_pe_task::run()
    {

				int ret = 0; //result from scanning.

        //struct optstruct *opts_ = (struct optstruct *)malloc(sizeof(optstruct *));

        cli_scanner_wrapper  cli_swrapper;

        cli_swrapper.set_filename_path(file_scanpath);
        ret = cli_swrapper.prepare_scandesc_wrapper(file_sigdb);
				const char * rec = cli_swrapper.result_code(ret);
				std::cout<<"Result code : " << rec << "In prescanning." <<std::endl;

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
								std::cout<<" Thread notify...: "<<std::endl;
                _monitor.notify();
            }
        }


    }


}
