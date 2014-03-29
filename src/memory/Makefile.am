# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src/ @AM_CXXFLAGS@

# Make src file 
# libarries for memory(SHM).
lib_LIBRARIES = libmemory.a

#@install header to system
libmemory_adir = $(includedir)/memory

#Header
libmemory_a_HEADERS = file_shm_handler.hpp 

# library
libmemory_a_SOURCES = $(libmemory_a_HEADERS)  file_shm_handler.cpp


