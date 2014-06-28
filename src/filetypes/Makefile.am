# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src/ -std=c++0x @AM_CXXFLAGS@

# Make src file 
# libarries for control threads
lib_LIBRARIES = libfiletypes.a

#@install header to system
libfiletypes_adir = $(includedir)/filetypes

#Header
libfiletypes_a_HEADERS = pe_file_controller.hpp

# library
libfiletypes_a_SOURCES = $(libfiletypes_a_HEADERS) pe_file_controller.cpp


