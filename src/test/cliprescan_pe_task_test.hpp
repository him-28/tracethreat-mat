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


};


TEST_F(CliPreScanPeTask, task_pe)
{

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

    for (size_t ix = 0; ix < count; ix++) {

        tasks.insert(shared_ptr<cliprescan_pe_task>(
                new cliprescan_pe_task(monitor, activeCount, timeout)));
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
