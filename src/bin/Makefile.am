AM_CXXFLAGS = -I$(top_srcdir)/src -I/usr/include -std=c++11 @AM_CXXFLAGS@

bin_PROGRAMS = tracethreat_mat

tracethreat_mat_SOURCES = tracethreat_mat.cpp \
 data_structure/ac_tire.hpp \
 scan/file_scan_policy.hpp \
 scan/pe_file_policy.hpp \
 utils/file_offset_handler.hpp \
 exception/system_exception.hpp \
 filetypes/pe_file_controller.hpp

tracethreat_mat_LDADD = $(top_builddir)/src/threadsyncocl/libthreadsyncocl.a \
$(top_builddir)/src/exception/libexception.a \
$(top_builddir)/src/data_structure/libdstr.a \
$(top_builddir)/src/filetypes/libfiletypes.a  \
$(top_builddir)/src/scan/libscan.a \
$(top_builddir)/src/memory/libmemory.a \
$(top_builddir)/src/threadconcurrency/libthreadconcurrency.a \
$(top_builddir)/src/taskconcurrency/libtaskconcurrency.a \
$(top_builddir)/src/clibasewrapper/libclibasewrapper.a \
$(top_builddir)/src/ocl/libocl.a \
$(top_builddir)/src/tbbscan/libtbbscan.a \
$(top_builddir)/src/filestructure/libfilestructure.a \
$(top_builddir)/src/gensign/libgensign.a \
$(top_builddir)/src/internet/client_fresh/libclientfresh.a \
$(top_builddir)/src/internet/scan_server/libscan_server.a \
$(top_builddir)/src/internet/scan_client/libscan_client.a \
$(top_builddir)/src/internet/security/libsecurity.a \
$(top_builddir)/src/internet/tracethreat/libtracethreat.a \
$(top_builddir)/src/utils/libutils.a \
$(top_builddir)/src/internet/msg/libmsg.a $(AM_LDFLAGS)
