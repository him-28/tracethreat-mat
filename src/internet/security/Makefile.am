# debug, optimized
AM_CXXFLAGS   = -g -I$(top_srcdir)/src/ -std=c++11 @AM_CXXFLAGS@

# Make src file 
# libarries connect to internel client and server mode.
lib_LIBRARIES = libsecurity.a

#@install header to system
libsecurity_adir = $(includedir)/internet/scan_server

#Header
libsecurity_a_HEADERS =  network_controller.hpp \
encryption.hpp \
encryption_service.hpp \
aes_controller.hpp \
aes_encryption.hpp \
compression_controller.hpp\
ip_controller.hpp \
uuid_controller.hpp \
load_security.hpp \
../msg/packedmessage_scan.hpp \
../msg/scan_server_client/message_scan.pb.h \
../logger/logging.hpp \
../logger/stringprintf.hpp

# Not debug, If you want to debug declare : ../msg/scan_server_client/message_scan.pb.cc 

# library
libsecurity_a_SOURCES = $(libsecurity_a_HEADERS) network_controller.cpp \
encryption_service.cpp \
aes_controller.cpp \
ip_controller.cpp \
uuid_controller.cpp \
../msg/packedmessage_scan.cpp \
../msg/scan_server_client/message_scan.pb.cc \
../logger/logging.cpp \
../logger/stringprintf.cpp
