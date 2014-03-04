AM_CXXFLAGS = -I$(top_srcdir)/src -I/usr/include @AM_CXXFLAGS@

bin_PROGRAMS = tsyncocl

tsyncocl_SOURCES = tsyncocl.cpp \
 data_structure/ac_tire.hpp \
 scan/file_scan_policy.hpp \
 scan/pe_file_policy.hpp \
 utils/file_offset_handler.hpp \
 exception/system_exception.hpp \
 filetypes/pe_file_controller.hpp

tsyncocl_LDADD = $(top_builddir)/src/threadsyncocl/libthreadsyncocl.a \
$(top_builddir)/src/utils/libutils.a \
$(top_builddir)/src/scan/libscan.a \
$(top_builddir)/src/exception/libexception.a \
$(top_builddir)/src/filetypes/libfiletypes.a \
$(AM_LDFLAGS)
