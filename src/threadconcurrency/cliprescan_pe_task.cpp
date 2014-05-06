
#include "exception/system_exception.hpp"
#include "threadconcurrency/cliprescan_pe_task.hpp"

namespace controller
{
		using namespace wrapper;

    cliprescan_pe_task::cliprescan_pe_task(monitor_controller& monitor, size_t& count, int64_t timeout) :
        _monitor(monitor),
        _count(count),
        _timeout(timeout),
        _done(false) {}

    void cliprescan_pe_task::run()
    {


        struct optstruct *opts_ = (struct optstruct *)malloc(sizeof(optstruct *));

        cli_scanner_wrapper  cli_swrapper;
				//cli_swrapper.init_engine_wrapper();
				int fd = 0;
				char * file_sigdb = "/home/chatsiri/Dropbox/reversing_engineer/write_sig/signature_trojan.ndb";
        // /home/chatsiri/Dropbox/reversing_engineer/reversing_files_test";
				char * file_scanpath = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_ext.exe";
				cli_swrapper.set_filename_path(file_scanpath);
				cli_swrapper.prepare_scandesc_wrapper(file_sigdb);

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
