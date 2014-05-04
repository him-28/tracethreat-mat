# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src/ @AM_CXXFLAGS@

# Make src file 
# libarries for thread concurrency file.
lib_LIBRARIES = libthreadconcurrency.a

#@install header to system
libthreadconcurrency_adir = $(includedir)/threadconcurrency

#Header
libthreadconcurrency_a_HEADERS = thread_manager.hpp \
thread.hpp \
posixthread_factory.hpp \
monitor_controller.hpp \
mutex_controller.hpp \
util_thread.hpp \
timer_controller.hpp \
cliprescan_pe_task.hpp
 

# library
libthreadconcurrency_a_SOURCES = $(libthreadconcurrency_a_HEADERS) thread_manager.cpp \
posixthread_factory.cpp \
monitor_controller.cpp \
mutex_controller.cpp \
util_thread.cpp \
timer_controller.cpp \
cliprescan_pe_task.cpp
