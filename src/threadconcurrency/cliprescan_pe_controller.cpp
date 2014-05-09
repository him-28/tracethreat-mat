#include "threadconcurrency/cliprescan_pe_controller.hpp"
#include "filetypes/pe.hpp"

namespace controller
{
    template<typename MAPPED_FILE>
    bool cliprescan_pe_controller<MAPPED_FILE>::
    initial_task_size(size_t thread_count,
            size_t timeout,
            std::vector<MAPPED_FILE *> * mapped_file_pe_vec)
    {

        typename std::vector<MAPPED_FILE *>::iterator iter_mapped_files;
        worker_count = mapped_file_pe_vec->size();
        task_count   = mapped_file_pe_vec->size();
        timeout_      = timeout;

        for(iter_mapped_files = mapped_file_pe_vec->begin();
                iter_mapped_files != mapped_file_pe_vec->end();
                ++iter_mapped_files) {
            MAPPED_FILE   *mf_pe = *iter_mapped_files;
            clipe_task = new cliprescan_pe_task(monitor, task_count, timeout_);
            clipe_task->set_file_scanpath(mf_pe->file_name); // set file infected.
            clipe_task->set_file_sigdb(mf_pe->file_sig); //set file signature path name
            tasks_scan_pe.insert(boost::shared_ptr<cliprescan_pe_task>(clipe_task));
        }

    }

    template<typename MAPPED_FILE>
    bool cliprescan_pe_controller<MAPPED_FILE>::task_start()
    {

        //prescan with clamav.
        boost::shared_ptr<thread_manager> thread_m =
                thread_manager::new_simple_thread_manager(worker_count);
        //set thread factory
        boost::shared_ptr<platformthread_factory> thread_factory =
                boost::shared_ptr<platformthread_factory>(
                        new platformthread_factory());
        //thread manager add thread factory
        thread_m->thread_factory(thread_factory);
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

    }
		template class cliprescan_pe_controller<MAPPED_FILE_PE>;
}
