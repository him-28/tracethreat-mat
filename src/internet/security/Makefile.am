# debug, optimized
AM_CXXFLAGS   = -g -I$(top_srcdir)/src/ -std=c++11 @AM_CXXFLAGS@

# Make src file 
# libarries connect to internel client and server mode.
lib_LIBRARIES = libsecurity.a

#@install header to system
libsecurity_adir = $(includedir)/internet/scan_server

#Header
libsecurity_a_HEADERS = encryption_controller.hpp \
security_connection.hpp \
aes_controller.hpp \
compression_controller.hpp\
ip_controller.hpp \
uuid_controller.hpp \
../msg/packedmessage_scan.hpp \
../msg/scan_server_client/message_scan.pb.h \
../logger/logging.hpp \
../logger/stringprintf.hpp

# Not debug, If you want to debug declare : ../msg/scan_server_client/message_scan.pb.cc 

# library
libsecurity_a_SOURCES = $(libsecurity_a_HEADERS) encryption_controller.cpp \
security_connection.cpp \
aes_controller.cpp \
ip_controller.cpp \
uuid_controller.cpp \
../msg/packedmessage_scan.cpp \
../msg/scan_server_client/message_scan.pb.cc \
../logger/logging.cpp \
../logger/stringprintf.cpp
