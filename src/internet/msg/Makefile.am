# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src/ -std=c++11 @AM_CXXFLAGS@

# Make src file 
# libarries connect to internel client and server mode.
lib_LIBRARIES = libmsg.a

#@install header to system
libmsg_adir = $(includedir)/internet/msg

#Header
libmsg_a_HEADERS = packedmessage_scan.hpp \
packedmessage_scan_client.hpp \
../logger/logging.hpp \
../logger/stringprintf.hpp

# Not debug, If you want to debug declare : ../msg/scan_server_client/message_scan.pb.cc 

# library
libmsg_a_SOURCES = $(libmsg_a_HEADERS) packedmessage_scan.cpp \
packedmessage_scan_client.cpp \
../logger/logging.cpp \
../logger/stringprintf.cpp
