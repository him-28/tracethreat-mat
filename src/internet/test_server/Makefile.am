AM_CXXFLAGS = -g -I$(top_srcdir)/src -I/src/threadsyncocl -std=c++0x @AM_CXXFLAGS@

TEST = test_server

check_PROGRAMS = test_server

test_server_SOURCES = test_server.cpp \
environment_test.hpp

test_server_LDADD = -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadsyncocl \
 -lthreadsyncocl -lgtest -lgtest_main \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/internet/scan_server -lscan_server \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils -lutils \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/exception -lexception $(AM_LDFLAGS)
