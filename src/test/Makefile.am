AM_CXXFLAGS = -I$(top_srcdir)/src -I/src/threadsyncocl @AM_CXXFLAGS@
# -I/src/utils -I/src/rules -I/src/filetypes \
# -I/src/scan -I/src/ocl -I/src/exception -I/src/data_structure  @AM_CXXFLAGS@

check_PROGRAMS = test_suite

test_suite_SOURCES = semaphore_controller_test.cpp \
environment_test.hpp \
thread_controller.hpp \
file_calculate.hpp \
thread_exception.hpp \
thread_barrier_controller.hpp \
ac_tire.hpp \
actire_parallel.hpp \
wrapper_rules.hpp \
yara.h \
file_handler.hpp \
file_offset_handler.hpp \
datastructure_exception.hpp

#cl_bootstrap.hpp

#pe_file_controller_test.hpp \
#file_policy_test.hpp 

test_suite_LDADD = -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/threadsyncocl \
 -lthreadsyncocl -lgtest -lgtest_main -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils \
 -lutils -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/exception \
 -lexception -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/data_structure \
 -ldstr -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/wrapper \
 -lwrapper  -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/rules \
 -lrules  -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/filetypes \
 -lfiletypes -L/home/chatsiri/workspacecpp/boost-truck/stage/lib \
 -lboost_system -lboost_program_options -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/scan \
 -lscan -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/memory \
 -lmemory -L/home/chatsiri/workspacecpp/pthread_sync_ocl/src/ocl \
 -locl $(AM_LDFLAGS)
