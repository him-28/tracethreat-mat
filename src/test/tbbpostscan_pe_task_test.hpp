#include "threadconcurrency/posixthread_factory.hpp"
#include "threadconcurrency/thread_manager.hpp"
#include "threadconcurrency/platformthread_factory.hpp"
#include "threadconcurrency/monitor_controller.hpp"
#include "threadconcurrency/util_thread.hpp"

#include "threadconcurrency/tbbpostscan_pe_task.hpp"

#include <assert.h>
#include <set>
#include <iostream>
#include <set>
#include <stdint.h>

using namespace controller;
using boost::shared_ptr;


TEST(TBBPostScanPETask, task_pe)
{
    char *file_scanpath = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_int.exe";
    char *file_sigdb = "/home/chatsiri/Dropbox/reversing_engineer/write_sig/signature_trojan.ndb";

    typedef tbbpostscan_pe_task  pe_task;
    monitor_controller monitor;

    size_t count = 10;
    int64_t timeout = 100LL;
    size_t workerCount = 4;

    size_t activeCount = count;

    shared_ptr<thread_manager> threadManager = thread_manager::new_simple_thread_manager(workerCount);

    shared_ptr<platformthread_factory> threadFactory =
            shared_ptr<platformthread_factory>(new platformthread_factory());


    threadManager->thread_factory(threadFactory);

    threadManager->start();

    std::set<shared_ptr<pe_task> > tasks;
    tbbpostscan_pe_task *tbbps_task;

    for (size_t ix = 0; ix < count; ix++) {
        tbbps_task =
                new tbbpostscan_pe_task
            (monitor, activeCount, timeout);

        tasks.insert(shared_ptr<pe_task>(tbbps_task));
    }

    int64_t time00 = util_thread::current_time();

    for (std::set<shared_ptr<pe_task> >
            ::iterator ix = tasks.begin();
            ix != tasks.end();
            ix++) {

        threadManager->add(*ix);
    }

    {
        synchronized s(monitor);

        while(activeCount > 0) {

            monitor.wait();
        }
    }


}
