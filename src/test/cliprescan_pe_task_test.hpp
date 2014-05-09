#include "threadconcurrency/posixthread_factory.hpp"
#include "threadconcurrency/thread_manager.hpp"
#include "threadconcurrency/platformthread_factory.hpp"
#include "threadconcurrency/monitor_controller.hpp"
#include "threadconcurrency/util_thread.hpp"

#include "threadconcurrency/cliprescan_pe_task.hpp"

#include <assert.h>
#include <set>
#include <iostream>
#include <set>
#include <stdint.h>

using namespace controller;
using boost::shared_ptr;

class CliPreScanPeTask : public ::testing::Test
{
    protected:
        virtual void SetUp() {
        }
};


TEST_F(CliPreScanPeTask, task_pe)
{
            char *file_scanpath = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_int.exe";
            char *file_sigdb = "/home/chatsiri/Dropbox/reversing_engineer/write_sig/signature_trojan.ndb";


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

    std::set<shared_ptr<cliprescan_pe_task> > tasks;
    cliprescan_pe_task *clipe_task;

    for (size_t ix = 0; ix < count; ix++) {
        clipe_task = new cliprescan_pe_task(monitor, activeCount, timeout);
        clipe_task->set_file_scanpath(file_scanpath);
        clipe_task->set_file_sigdb(file_sigdb);
        tasks.insert(shared_ptr<cliprescan_pe_task>(clipe_task));
    }

    int64_t time00 = util_thread::current_time();

    for (std::set<shared_ptr<cliprescan_pe_task> >::iterator ix = tasks.begin();
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
