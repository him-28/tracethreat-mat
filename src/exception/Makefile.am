# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src @AM_CXXFLAGS@

# Make src file 
# libarries for control threads
lib_LIBRARIES = libexception.a

#@install header to system
libexception_adir = $(includedir)/utils

#Header
libexception_a_HEADERS = failure_handler_code.hpp system_exception.hpp

# library
libexception_a_SOURCES = $(libexception_a_HEADERS) failure_handler_code.cpp system_exception.cpp


