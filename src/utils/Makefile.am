# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src @AM_CXXFLAGS@

# Make src file 
# libarries for control threads
lib_LIBRARIES = libutils.a

#@install header to system
libutils_adir = $(includedir)/utils

#Header
libutils_a_HEADERS = file_calculate.hpp file_handler.hpp config/options_system.hpp logger/clutil_logger.hpp file_offset_handler.hpp 

# library
libutils_a_SOURCES = $(libutils_a_HEADERS) file_calculate.cpp  logger/format_logger.hpp file_handler.cpp file_offset_handler.cpp


