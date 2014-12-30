AM_CXXFLAGS = -g -I$(top_srcdir)/src -I/src/threadsyncocl -std=c++11 @AM_CXXFLAGS@

TEST = test_security

check_PROGRAMS = test_security

test_security_SOURCES = test_security.cpp \
../security/aes_controller.cpp \
environment_test.hpp

test_security_LDADD = -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadsyncocl \
 -lthreadsyncocl -lgtest -lgtest_main -lrocksdb \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/internet/security -lsecurity \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/internet/msg -lmsg \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils -lutils \
 -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/exception -lexception $(AM_LDFLAGS)
