# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src/ -std=c++11 @AM_CXXFLAGS@

# Make src file 
# libarries connect to internel client and server mode.
lib_LIBRARIES = libscan_server.a

#@install header to system
libscan_server_adir = $(includedir)/internet/scan_server

#Header
libscan_server_a_HEADERS = scan_server.hpp \
scan_connection.hpp \
packedmessage_scan.hpp \
security_connection.hpp \
ip_controller.hpp \
../msg/scan_server_client/message_scan.pb.h \
../logger/logging.hpp \
../logger/stringprintf.hpp

# Not debug, If you want to debug declare : ../msg/scan_server_client/message_scan.pb.cc 

# library
libscan_server_a_SOURCES = $(libscan_server_a_HEADERS) scan_server.cpp \
scan_connection.cpp \
packedmessage_scan.cpp \
security_connection.cpp \
ip_controller.cpp \
../msg/scan_server_client/message_scan.pb.cc \
../logger/logging.cpp \
../logger/stringprintf.cpp


