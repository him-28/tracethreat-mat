AM_CXXFLAGS = -g -I$(top_srcdir)/src -I/src/threadsyncocl -std=c++11 @AM_CXXFLAGS@

TEST = test_suite

check_PROGRAMS = test_suite

#test_suite_SOURCES = semaphore_controller_test.cpp \
#environment_test.hpp

test_suite_SOURCES = test_suite.cpp \
environment_test.hpp


test_suite_LDADD = -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadsyncocl \
 -lthreadsyncocl -lgtest -lgtest_main -ltbb_debug  \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils -lutils \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/exception -lexception \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/data_structure -ldstr \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/filetypes -lfiletypes \
 -L/home/chatsiri/sda1/workspacecpp/boost_1_57_0/stage/lib \
 -lboost_filesystem-mt -lboost_system-mt -lboost_program_options-mt -lboost_regex-mt \
 -lboost_thread-mt -lboost_log-mt -lboost_exception-mt\
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/scan -lscan \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/memory -lmemory \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/ocl -locl \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadconcurrency -lthreadconcurrency \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/clibasewrapper  -lclibasewrapper \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/tbbscan -ltbbscan $(AM_LDFLAGS)
