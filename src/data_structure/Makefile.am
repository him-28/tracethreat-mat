#makefile for build tire 
AM_CXXFLAGS   = -I$(top_srcdir)/src @AM_CXXFLAGS@

# Make src file 
# libarries for control threads
lib_LIBRARIES = libdstr.a

#@install header to system
libdstr_adir = $(includedir)/data_structure

#Header
libdstr_a_HEADERS = ac_tire.hpp

# library
libdstr_a_SOURCES = $(libdstr_a_HEADERS) ac_tire.cpp
