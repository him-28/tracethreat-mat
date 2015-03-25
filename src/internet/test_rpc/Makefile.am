AM_CXXFLAGS = -g -I$(top_srcdir)/src -I/src/threadsyncocl -std=c++11 @AM_CXXFLAGS@

TEST = test_rpc

check_PROGRAMS = test_rpc

test_rpc_SOURCES = test_rpc.cpp \
../tracethreat/infected_controller.hpp \
environment_test.hpp

test_rpc_LDADD = -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadsyncocl \
 -lthreadsyncocl -lgtest -lgtest_main -lrocksdb -ltracethreat_client-0.0.1 \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils -lutils \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/internet/tracethreat -ltracethreat \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/exception -lexception $(AM_LDFLAGS)
