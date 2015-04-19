# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src/ -std=c++11 @AM_CXXFLAGS@

# Make src file 
# libarries for memory(SHM).
lib_LIBRARIES = libmemory.a

#@install header to system
libmemory_adir = $(includedir)/memory

#Header
libmemory_a_HEADERS = file_shm_handler.hpp \
signature_shm_base.hpp \
signature_shm_controller.hpp \
../msg/message_tracethreat.pb.h \
queue_shm_controller.hpp

# library
libmemory_a_SOURCES = $(libmemory_a_HEADERS)  file_shm_handler.cpp \
signature_shm_controller.cpp \
../msg/message_tracethreat.pb.cc \
queue_shm_controller.cpp

