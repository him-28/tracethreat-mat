# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src @AM_CXXFLAGS@

# Make src file 
# libarries for control threads
lib_LIBRARIES = libthreadsyncocl.a

#@install header to system
libthreadsyncocl_adir = $(includedir)/threadsyncocl

#Header
libthreadsyncocl_a_HEADERS = thread_sync.hpp

# library
libthreadsyncocl_a_SOURCES = $(libthreadsyncocl_a_HEADERS) thread_sync.cpp thread_controller.cpp

