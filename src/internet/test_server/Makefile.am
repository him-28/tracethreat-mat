AM_CXXFLAGS = -g -I$(top_srcdir)/src -I/src/threadsyncocl -std=c++11 @AM_CXXFLAGS@

TEST = test_server

check_PROGRAMS = test_server

test_server_SOURCES = test_server.cpp \
environment_test.hpp

test_server_LDADD = -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadsyncocl \
 -lthreadsyncocl -lgtest -lgtest_main -lrocksdb \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/internet/scan_server -lscan_server \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils -lutils \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/scan/  -lscan \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/tbbscan/  -ltbbscan \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/ocl  -locl \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/gensign  -lgensign \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadconcurrency -lthreadconcurrency \
 -L/home/chatsiri/workspacecpp/tbb/tbb41_20121003oss/build/linux_intel64_gcc_cc4.6_libc2.15_kernel3.2.0_debug -ltbb_debug \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/exception -lexception $(AM_LDFLAGS)
