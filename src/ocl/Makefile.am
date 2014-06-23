AM_CXXFLAGS   = -I$(top_srcdir)/src @AM_CXXFLAGS@

# Make src file 
# libarries for control threads
lib_LIBRARIES = libocl.a

#@install header to system
libocl_adir = $(includedir)/ocl

#Header
libocl_a_HEADERS = cl_bootstrap.hpp \
utils/clutil_memory.hpp \
utils/clutil_commandqueue.hpp \
utils/clutil_platform.hpp

# library
libocl_a_SOURCES = $(libocl_a_HEADERS) cl_bootstrap.cpp \
utils/clutil_memory.cpp \
utils/clutil_commandqueue.cpp \
utils/clutil_platform.cpp
