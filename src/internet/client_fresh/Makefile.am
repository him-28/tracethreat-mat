# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src/ -std=c++0x @AM_CXXFLAGS@

# Make src file 
# libarries connect to internel client and server mode.
lib_LIBRARIES = libclientfresh.a

#@install header to system
libclientfresh_adir = $(includedir)/internet/client_fresh

#Header
libclientfresh_a_HEADERS = updated_sign.hpp \
../logger/logging.hpp \
../logger/stringprintf.hpp

# library
libclientfresh_a_SOURCES = $(libclientfresh_a_HEADERS) updated_sign.cpp \
../logger/logging.cpp \
../logger/stringprintf.cpp
