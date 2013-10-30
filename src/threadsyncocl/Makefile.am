# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src  @AM_CXXFLAGS@

# Make src file 
# libarries for control threads
lib_LIBRARIES = libthreadsyncocl.a 

#@install header to system
libthreadsyncocl_adir = $(includedir)/threadsyncocl

#Header
libthreadsyncocl_a_HEADERS = thread_sync.hpp thread_controller.hpp $(top_builddir)/src/utils/file_calculate.hpp  $(top_builddir)/src/exception/failure_handler_code.hpp $(top_builddir)/src/data_structure/ac_tire.hpp


# library
libthreadsyncocl_a_SOURCES = $(libthreadsyncocl_a_HEADERS) thread_sync.cpp thread_controller.cpp buffer_sync.cpp semaphore_controller.cpp thread_barrier_controller.cpp $(top_builddir)/src/utils/file_calculate.cpp $(top_builddir)/src/exception/failure_handler_code.cpp $(top_builddir)/src/data_structure/ac_tire.hpp

libthreadsyncocl_a_LIBADD = $(top_builddir)/src/utils/libutils.a $(top_builddir)/src/exception/libexception.a $(top_builddir)/src/data_structure/libdstr.a $(AM_LDFLAGS)
