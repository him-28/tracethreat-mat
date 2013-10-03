AM_CXXFLAGS   = -I$(top_srcdir)/src @AM_CXXFLAGS@

# Make src file 
# libarries for control threads
lib_LIBRARIES = libocl.a

#@install header to system
libocl_adir = $(includedir)/ocl

#Header
libocl_a_HEADERS = cl_bootstrap.hpp

# library
libocl_a_SOURCES = $(libocl_a_HEADERS) cl_bootstrap.hpp cl_handel_memqueue.hpp 
