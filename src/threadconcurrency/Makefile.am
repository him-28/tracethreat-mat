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
cliprescan_pe_task.hpp \
cliprescan_pe_controller.hpp

#cliprescan_pe_controller.hpp
#$(top_builddir)/src/clibasewrapper/cliwrapper/cli_scanner_wrapper.h \
#$(top_builddir)/src/clibasewrapper/cliwrapper/cli_others_common_wrapper.h \
#$(top_builddir)/src/clibasewrapper/cliwrapper/global_wrapper.h



# library
libthreadconcurrency_a_SOURCES = $(libthreadconcurrency_a_HEADERS) thread_manager.cpp \
posixthread_factory.cpp \
monitor_controller.cpp \
mutex_controller.cpp \
util_thread.cpp \
timer_controller.cpp \
cliprescan_pe_task.cpp \
cliprescan_pe_controller.cpp

#cliprescan_pe_controller.cpp

#$(top_builddir)/src/clibasewrapper/cliwrapper/cli_scanner_wrapper.cpp \
#$(top_builddir)/src/clibasewrapper/cliwrapper/cli_others_common_wrapper.cpp


#libthreadconcurrency_a_LIBADD = $(top_builddir)/src/clibasewrapper/libclibasewrapper.a $(AM_LDFLAGS)
