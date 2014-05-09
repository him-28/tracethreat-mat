AM_CXXFLAGS = -I$(top_srcdir)/src -I/src/threadsyncocl @AM_CXXFLAGS@
# -I/src/clibasewrapper 
# -I/src/utils -I/src/rules -I/src/filetypes \
# -I/src/scan -I/src/ocl -I/src/exception -I/src/data_structure  @AM_CXXFLAGS@

TEST = test_suite

check_PROGRAMS = test_suite

test_suite_SOURCES = semaphore_controller_test.cpp \
environment_test.hpp
#thread_controller.hpp \
#file_calculate.hpp \
#thread_exception.hpp \
#thread_barrier_controller.hpp \
#ac_tire.hpp \
#actire_parallel.hpp \
#file_handler.hpp \
#file_offset_handler.hpp \
#datastructure_exception.hpp \
#libclamav/clamav.h \
#shared/optparser.h

#cl_bootstrap.hpp

#pe_file_controller_test.hpp \
#file_policy_test.hpp 

test_suite_LDADD = -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadsyncocl \
 -lthreadsyncocl -lgtest -lgtest_main -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils \
 -lutils -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/exception \
 -lexception -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/data_structure \
 -ldstr -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/wrapper \
 -lwrapper  -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/filetypes \
 -lfiletypes -L/home/chatsiri/workspacecpp/boost-truck/stage/lib \
 -lboost_system -lboost_program_options -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/scan \
 -lscan -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/memory \
 -lmemory -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/ocl \
 -locl -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadconcurrency \
 -lthreadconcurrency -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/clibasewrapper \
 -lclibasewrapper $(AM_LDFLAGS)

#file_policy
TEST += file_policy_test
check_PROGRAMS += file_policy_test

file_policy_test_SOURCES = file_policy_test.cpp \
environment_test.hpp

file_policy_test_LDADD   = -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadsyncocl \
 -lthreadsyncocl -lgtest -lgtest_main -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils \
 -lutils -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/exception \
 -lexception -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/data_structure \
 -ldstr -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/wrapper \
 -lwrapper  -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/filetypes \
 -lfiletypes -L/home/chatsiri/workspacecpp/boost-truck/stage/lib \
 -lboost_system -lboost_program_options -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/scan \
 -lscan -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/memory \
 -lmemory -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/ocl \
 -locl -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadconcurrency \
 -lthreadconcurrency -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/clibasewrapper \
 -lclibasewrapper $(AM_LDFLAGS)


#scan_pe_file include actire_parallel
TEST += scan_pe_file_actire_parallel
check_PROGRAMS += scan_pe_file_acp_test

scan_pe_file_acp_test_SOURCES = scan_pe_file_acp_test.cpp \
environment_test.hpp

scan_pe_file_acp_test_LDADD   = -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadsyncocl \
 -lthreadsyncocl -lgtest -lgtest_main -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils \
 -lutils -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/exception \
 -lexception -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/data_structure \
 -ldstr -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/wrapper \
 -lwrapper  -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/filetypes \
 -lfiletypes -L/home/chatsiri/workspacecpp/boost-truck/stage/lib \
 -lboost_system -lboost_program_options -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/scan \
 -lscan -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/memory \
 -lmemory -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/ocl \
 -locl -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadconcurrency \
 -lthreadconcurrency -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/clibasewrapper \
 -lclibasewrapper $(AM_LDFLAGS)
