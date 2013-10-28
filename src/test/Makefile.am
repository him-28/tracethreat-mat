AM_CXXFLAGS = -I$(top_srcdir)/src -I/src/threadsyncocl -I/src/utils @AM_CXXFLAGS@

check_PROGRAMS = test_suite

test_suite_SOURCES = semaphore_controller_test.cpp thread_controller.hpp file_calculate.hpp thread_exception.hpp  thread_barrier_controller.hpp ac_tire.hpp

test_suite_LDADD = -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadsyncocl -lthreadsyncocl -lgtest -lgtest_main -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils -lutils -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/exception -lexception -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/data_structure/ -ldstr -L/home/chatsiri/workspacecpp/boost-truck/stage/lib -lboost_system -lboost_program_options $(AM_LDFLAGS)
