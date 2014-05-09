#ifndef CONTROLLER_CLIPRESCAN_PE_TASK_HPP
#define CONTROLLER_CLIPRESCAN_PE_TASK_HPP

#include "threadconcurrency/thread_manager.hpp"
#include "threadconcurrency/monitor_controller.hpp"
#include "threadconcurrency/platformthread_factory.hpp"
#include "threadconcurrency/util_thread.hpp"

#include "clibasewrapper/cliwrapper/cli_scanner_wrapper.h"
//
namespace controller
{
    namespace exceptions = hnmav_exception::controller;

    class cliprescan_pe_task : public Runnable
    {
        public:
            cliprescan_pe_task(monitor_controller& monitor, size_t& count, int64_t timeout);

            bool set_file_scanpath(const char *file_scanpath);

            bool set_file_sigdb(const char *file_sigdb);
						//support multiple file scanning. File name send from struct of File-SHM.			
						//bool set_file_scanpath(std::vector<const char*>  file_scanpath_vec);

            void run();
        private:

            const char *file_sigdb;
            const char *file_scanpath;

            monitor_controller& _monitor;
            size_t& _count;
            int64_t _timeout;
            int64_t _startTime;
            int64_t _endTime;
            bool _done;
            monitor_controller _sleep;
    };

}

#endif /* CONTROLLER_CLIPRESCAN_PE_TASK_HPP */
