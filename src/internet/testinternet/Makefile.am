AM_CXXFLAGS = -g -I$(top_srcdir)/src -I/src/threadsyncocl -std=c++0x @AM_CXXFLAGS@

TEST = test_internet

check_PROGRAMS = test_internet

test_internet_SOURCES = test_internet.cpp \
environment_test.hpp

test_internet_LDADD = -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadsyncocl \
 -lthreadsyncocl -lgtest -lgtest_main \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/internet/client_fresh -lclientfresh \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils -lutils \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/exception -lexception $(AM_LDFLAGS)
