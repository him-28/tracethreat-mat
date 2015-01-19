#ifndef CONTROLLER_CLIPRESCAN_PE_CONTROLLER_HPP
#define CONTROLLER_CLIPRESCAN_PE_CONTROLLER_HPP

#include <set>

#include "taskconcurrency/buffer_sync_tbb.hpp"

#include "threadconcurrency/monitor_controller.hpp"

#include "taskconcurrency/cliprescan_pe_task.hpp"

namespace controller
{

		namespace h_util = hnmav_util;

    template<typename MAPPED_FILE>
    class cliprescan_pe_controller
    {

        public:
						//cliprescan_pe_controller();

            bool initial_task_size(size_t thread_count,
                    int64_t timeout,
                    std::vector<MAPPED_FILE *> * mappe_file_pe_vec);

            bool task_start();

        private:

            //worker is equal size of task
            size_t  worker_count;
            size_t  task_count;
            //set timeout prescanning.
            int64_t timeout_;

            cliprescan_pe_task *clipe_task;
            std::set<boost::shared_ptr<cliprescan_pe_task> > tasks_scan_pe;
            //monitoring controller
            monitor_controller monitor;


						//logger
            //boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            //h_util::clutil_logging<std::string, int>    *logger;

    };

}




#endif /* CONTROLLER_CLIPRESCAN_PE_CONTROLLER */
