# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src -std=c++11 @AM_CXXFLAGS@

#AM_LDFLAGS = -L/usr/lib/x86_64-linux-gnu/ -ldl @AM_LDFLAGS@

# Make src file 
# libarries for control threads
lib_LIBRARIES = libtracethreat.a

#@install header to system
libtracethreat_adir = $(includedir)/internet/tracethreat

#Header
libtracethreat_a_HEADERS = infected_controller.hpp \
load_tracethreat.hpp \
infected_tracethreat.hpp \
tracethreat.hpp \
../../msg/message_tracethreat.pb.h

# library
libtracethreat_a_SOURCES = $(libtracethreat_a_HEADERS) infected_controller.cpp \
../../msg/message_tracethreat.pb.cc 
