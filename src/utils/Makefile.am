# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src -std=c++11 @AM_CXXFLAGS@

#AM_LDFLAGS = -L/usr/lib/x86_64-linux-gnu/ -ldl @AM_LDFLAGS@

# Make src file 
# libarries for control threads
lib_LIBRARIES = libutils.a

#@install header to system
libutils_adir = $(includedir)/utils

#Header
libutils_a_HEADERS = file_calculate.hpp \
file_handler.hpp \
config/options_system.hpp \
logger/format_logger.hpp \
logger/clutil_logger.hpp \
base/system_code.hpp \
base/timer_queue.hpp \
../msg/message_tracethreat.pb.h \
connector/result_infected.hpp \
file_offset_handler.hpp \
convert.hpp \
md5_hash.hpp \
uuid_generator.hpp \
hash.hpp \
timestamp.hpp

# library
libutils_a_SOURCES = $(libutils_a_HEADERS) file_calculate.cpp \
file_handler.cpp \
config/options_system.cpp \
logger/format_logger.cpp \
logger/clutil_logger.cpp \
base/timer_queue.cpp \
connector/result_infected.cpp \
file_offset_handler.cpp \
md5_hash.cpp \
uuid_generator.cpp \
hash.cpp \
timestamp.cpp
