
//#include "utils/logger/clutil_logger.hpp"
//#include "utils/base/system_code.hpp"

#include "taskconcurrency/cliprescan_pe_controller.hpp"
#include "filetypes/pe.hpp"

namespace controller
{
		/*	
		template<typename MAPPED_FILE>
		cliprescan_pe_controller<MAPPED_FILE>::cliprescan_pe_controller(){

				//logger_ptr = &h_util::clutil_logging<std::string, int>:: get_instance();
        //logger = logger_ptr->get();

		}
		*/

    template<typename MAPPED_FILE>
    bool cliprescan_pe_controller<MAPPED_FILE>::
    initial_task_size(size_t thread_count,
            int64_t timeout,
            std::vector<MAPPED_FILE *> * mapped_file_pe_vec)
    {
			  //logger->write_info("cliprescan_pe_controller::initial_task_size, Initial worker (default 4)");

        typename std::vector<MAPPED_FILE *>::iterator iter_mapped_files;
        worker_count = mapped_file_pe_vec->size();
        task_count   = mapped_file_pe_vec->size();
        timeout_      = timeout;

        for(iter_mapped_files = mapped_file_pe_vec->begin();
                iter_mapped_files != mapped_file_pe_vec->end();
                ++iter_mapped_files) {
            MAPPED_FILE   *mf_pe = *iter_mapped_files;
            clipe_task = new cliprescan_pe_task(monitor, task_count, timeout_);
            clipe_task->set_file_scanpath(mf_pe->file_name.c_str()); // set file infected.
            clipe_task->set_file_sigdb(mf_pe->file_sig.c_str()); //set file signature path name
            tasks_scan_pe.insert(boost::shared_ptr<cliprescan_pe_task>(clipe_task));

						//logger->write_info("cliprescan_pe_controller::initial_task_size, Insert file_name completed", 
						//			boost::lexical_cast<std::string>(mf_pe->file_sig));
        }

    }

    template<typename MAPPED_FILE>
    bool cliprescan_pe_controller<MAPPED_FILE>::task_start()
    {
				//logger->write_info("cliprescan_pe_controller::initial_task_size, Task start");
				//worker_count = 4;
        //prescan with clamav.
        boost::shared_ptr<thread_manager> thread_m =
                thread_manager::new_simple_thread_manager(worker_count);
        //set thread factory
        /*
        boost::shared_ptr<platformthread_factory> thread_factory =
                boost::make_shared<platformthread_factory>(
                        new platformthread_factory());
				*/
        //thread manager add thread factory
        thread_m->thread_factory(boost::shared_ptr<platformthread_factory>(
                        new platformthread_factory()));
        //thread start.
        thread_m->start();

        for(std::set<boost::shared_ptr<cliprescan_pe_task> >
                ::iterator ix = tasks_scan_pe.begin();
                ix != tasks_scan_pe.end();
                ++ix) {
            thread_m->add(*ix);
        }

        {
            synchronized  s(monitor);

            while(task_count) {

                monitor.wait();
            }

        }

			 //logger->write_info("cliprescan_pe_controller::initial_task_size, Worker run completed.");
    }
		template class cliprescan_pe_controller<MAPPED_FILE_PE>;
}
