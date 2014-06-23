# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src/ @AM_CXXFLAGS@

# Make src file 
# libarries for scan file
lib_LIBRARIES = libscan.a

#@install header to system
libscan_adir = $(includedir)/scan

#Header
libscan_a_HEADERS = file_scan_policy.hpp 

# library
libscan_a_SOURCES = $(libscan_a_HEADERS)  file_scan_policy.cpp


