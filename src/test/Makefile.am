AM_CXXFLAGS = -I$(top_srcdir)/src -I/src/threadsyncocl @AM_CXXFLAGS@

check_PROGRAMS = test_suite

test_suite_SOURCES = semaphore_controller_test.cpp

test_suite_LDADD = -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadsyncocl -lthreadsyncocl -lgtest -lgtest_main $(AM_LDFLAGS)
