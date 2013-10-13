# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src  @AM_CXXFLAGS@

# Make src file 
# libarries for control threads
lib_LIBRARIES = liblogger.a 

#@install header to system
liblogger_adir = $(includedir)/logger

#Header
liblogger_a_HEADERS = clutil_logger.hpp


# library
liblogger_a_SOURCES = $(liblogger_a_HEADERS) format_logger.hpp
