
#include "exception/system_exception.hpp"

#include "taskconcurrency/cliprescan_pe_task.hpp"


namespace controller
{
    using namespace wrapper;
/*
    template<typename MAPPED_FILE>
    cliprescan_pe_task<MAPPED_FILE>::cliprescan_pe_task()
    {


    }
*/

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
		/*
    bool cliprescan_pe_task::task_start(size_t thread_count,
            int64_t timeout,
            std::vector<MAPPED_FILE *> *mapped_file_pe_vec)
    {
        
        size_t  worker_count;
        size_t  task_count;
        //set timeout prescanning.
        int64_t timeout_;

        cliprescan_pe_task *clipe_task;
        std::set<boost::shared_ptr<cliprescan_pe_task> > tasks_scan_pe;
        //monitoring controller
        monitor_controller monitor;

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

        boost::shared_ptr<thread_manager> thread_m =
                thread_manager::new_simple_thread_manager(worker_count);
        //set thread factory
        
         boost::shared_ptr<platformthread_factory> thread_factory =
                 boost::make_shared<platformthread_factory>(
                         new platformthread_factory());
        		
        //thread manager add thread factory
        thread_m->thread_factory(boost::shared_ptr<platformthread_factory>(
                new platformthread_factory()));
        //thread start.
        thread_m->start();

        for(typename std::set<boost::shared_ptr<cliprescan_pe_task<MAPPED_FILE> > >
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
		*/

   // }

    void cliprescan_pe_task::run()
    {

        int ret = 0; //result from scanning.

        //struct optstruct *opts_ = (struct optstruct *)malloc(sizeof(optstruct *));

        cli_scanner_wrapper  cli_swrapper;

        cli_swrapper.set_filename_path(file_scanpath);
        //ret = cli_swrapper.prepare_scandesc_wrapper(file_sigdb);
        //const char *rec = cli_swrapper.result_code(ret);
        //std::cout<<"Result code : " << rec << "In prescanning." <<std::endl;

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
