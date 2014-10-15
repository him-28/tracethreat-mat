AM_CXXFLAGS = -g -I$(top_srcdir)/src -I/src/threadsyncocl -std=c++0x @AM_CXXFLAGS@

TEST = test_client

check_PROGRAMS = test_client

test_client_SOURCES = test_client.cpp \
environment_test.hpp

test_client_LDADD = -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadsyncocl \
 -lthreadsyncocl -lgtest -lgtest_main \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/internet/client_fresh -lclientfresh \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/internet/scan_client -lscan_client \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils -lutils \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/exception -lexception $(AM_LDFLAGS)
