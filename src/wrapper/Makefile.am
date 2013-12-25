# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src @AM_CXXFLAGS@

# Make src file 
# libarries for control threads
lib_LIBRARIES = libwrapper.a

#@install header to system
libwrapper_adir = $(includedir)/wrapper

#Header
libwrapper_a_HEADERS = wrapper_rules.hpp

# library
libwrapper_a_SOURCES = $(libwrapper_a_HEADERS) wrapper_rules.cpp


