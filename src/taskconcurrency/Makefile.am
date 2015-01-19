# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src/ @AM_CXXFLAGS@

# Make src file 
# libarries for task concurrency file.
lib_LIBRARIES = libtaskconcurrency.a

#@install header to system
libtaskconcurrency_adir = $(includedir)/taskconcurrency

#Header
libtaskconcurrency_a_HEADERS = cliprescan_pe_task.hpp \
buffer_sync.hpp \
buffer_sync_tbb.hpp \
tbbpostscan_pe_task.hpp \
cliprescan_pe_controller.hpp \
tbbpostscan_pe_controller.hpp

# library
libtaskconcurrency_a_SOURCES = $(libtaskconcurrency_a_HEADERS) cliprescan_pe_task.cpp \
buffer_sync.cpp \
buffer_sync_tbb.cpp \
tbbpostscan_pe_task.cpp \
cliprescan_pe_controller.cpp \
tbbpostscan_pe_controller.cpp
