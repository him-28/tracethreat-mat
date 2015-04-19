# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src -I/home/chatsiri/sda1/workspacecpp/clamav-devel-hnmav -I/home/chatsiri/sda1/workspacecpp/treadstoneproject/tracethreat_client/src -std=c++11  @AM_CXXFLAGS@

# Make src file 
# libarries for control threads
lib_LIBRARIES = libthreadsyncocl.a 

#@install header to system
libthreadsyncocl_adir = $(includedir)/threadsyncocl

#Header  $(top_builddir)/src/internet/scan_client/packedmessage_scan_client.hpp \
libthreadsyncocl_a_HEADERS = thread_sync.hpp thread_controller.hpp \
 $(top_builddir)/src/utils/file_calculate.hpp \
 $(top_builddir)/src/utils/base/timer_queue.hpp \
 $(top_builddir)/src/utils/hash.hpp \
 $(top_builddir)/src/exception/datastructure_exception.hpp \
 $(top_builddir)/src/exception/failure_handler_code.hpp \
 $(top_builddir)/src/data_structure/ac_tire.hpp \
 $(top_builddir)/src/data_structure/actire_parallel.hpp \
 $(top_builddir)/src/filetypes/pe_file_controller.hpp \
 $(top_builddir)/src/scan/file_scan_policy.hpp \
 $(top_builddir)/src/scan/scan_file_controller.hpp \
 $(top_builddir)/src/ocl/cl_bootstrap.hpp \
 $(top_builddir)/src/ocl/utils/clutil_commandqueue.hpp \
 $(top_builddir)/src/memory/file_shm_handler.hpp \
 $(top_builddir)/src/memory/signature_shm_base.hpp \
 $(top_builddir)/src/memory/signature_shm_controller.hpp \
 $(top_builddir)/src/memory/queue_shm_controller.hpp \
 $(top_builddir)/src/threadconcurrency/thread_manager.hpp \
 $(top_builddir)/src/threadconcurrency/platformthread_factory.hpp \
 $(top_builddir)/src/threadconcurrency/monitor_controller.hpp \
 $(top_builddir)/src/threadconcurrency/posixthread_factory.hpp \
 $(top_builddir)/src/threadconcurrency/mutex_controller.hpp \
 $(top_builddir)/src/taskconcurrency/cliprescan_pe_task.hpp \
 $(top_builddir)/src/taskconcurrency/cliprescan_pe_controller.hpp \
 $(top_builddir)/src/taskconcurrency/tbbpostscan_pe_task.hpp \
 $(top_builddir)/src/taskconcurrency/tbbpostscan_pe_controller.hpp \
 $(top_builddir)/src/taskconcurrency/buffer_sync_tbb.hpp \
 $(top_builddir)/src/taskconcurrency/buffer_sync.hpp \
 $(top_builddir)/src/clibasewrapper/cliwrapper/cli_scanner_wrapper.h \
 $(top_builddir)/src/clibasewrapper/cliwrapper/cli_others_common_wrapper.h \
 $(top_builddir)/src/clibasewrapper/cliwrapper/global_wrapper.h \
 $(top_builddir)/src/tbbscan/data_structure/actire_concurrency.hpp \
 $(top_builddir)/src/tbbscan/data_structure/bloomfliter_concurrency.hpp \
 $(top_builddir)/src/tbbscan/data_structure/filter_policy.hpp \
 $(top_builddir)/src/tbbscan/data_structure/slice.hpp \
 $(top_builddir)/src/filestructure/pe_layout_controller.hpp \
 $(top_builddir)/src/filestructure/logger/logging.hpp \
 $(top_builddir)/src/filestructure/logger/stringprintf.hpp \
 $(top_builddir)/src/gensign/clamavsig.hpp \
 $(top_builddir)/src/utils/connector/result_infected.hpp \
 $(top_builddir)/src/internet/client_fresh/updated_sign.hpp \
 $(top_builddir)/src/internet/scan_server/scan_server.hpp \
 $(top_builddir)/src/internet/scan_server/scan_connection.hpp \
 $(top_builddir)/src/internet/security/uuid_controller.hpp \
 $(top_builddir)/src/internet/security/ip_controller.hpp \
 $(top_builddir)/src/internet/security/aes_controller.hpp \
 $(top_builddir)/src/internet/security/encryption.hpp \
 $(top_builddir)/src/internet/security/encryption_field.hpp \
 $(top_builddir)/src/internet/security/aes_encryption.hpp \
 $(top_builddir)/src/internet/tracethreat/infected_tracethreat.hpp \
 $(top_builddir)/src/internet/tracethreat/infected_controller.hpp \
 $(top_builddir)/src/internet/tracethreat/treacethreat.hpp \
 $(top_builddir)/src/internet/scan_client/scan_client.hpp \
 $(top_builddir)/src/internet/scan_client/scan_dir.hpp \
 $(top_builddir)/src/internet/scan_client/basic_scan_dir_service.hpp \
 $(top_builddir)/src/internet/scan_client/basic_scan_dir.hpp \
 $(top_builddir)/src/internet/scan_client/scan_dir_impl.hpp \
 $(top_builddir)/src/internet/msg/packedmessage_scan_client.hpp \
 $(top_builddir)/src/internet/msg/packedmessage_scan.hpp \
 $(top_builddir)/src/msg/message_tracethreat.pb.h


# library
libthreadsyncocl_a_SOURCES = $(libthreadsyncocl_a_HEADERS) thread_sync.cpp thread_controller.cpp semaphore_controller.cpp thread_barrier_controller.cpp \
	$(top_builddir)/src/utils/file_calculate.cpp  \
  $(top_builddir)/src/utils/base/timer_queue.cpp \
  $(top_builddir)/src/utils/hash.cpp \
 	$(top_builddir)/src/exception/failure_handler_code.cpp \
  $(top_builddir)/src/exception/datastructure_exception.cpp \
	$(top_builddir)/src/data_structure/ac_tire.cpp \
	$(top_builddir)/src/data_structure/actire_parallel.cpp \
	$(top_builddir)/src/filetypes/pe_file_controller.cpp \
  $(top_builddir)/src/scan/file_scan_policy.cpp \
  $(top_builddir)/src/scan/scan_file_controller.cpp \
  $(top_builddir)/src/ocl/cl_bootstrap.cpp \
  $(top_builddir)/src/ocl/utils/clutil_commandqueue.cpp \
  $(top_builddir)/src/memory/file_shm_handler.cpp \
  $(top_builddir)/src/memory/signature_shm_controller.cpp \
  $(top_builddir)/src/memory/queue_shm_controller.cpp \
  $(top_builddir)/src/threadconcurrency/thread_manager.cpp \
  $(top_builddir)/src/threadconcurrency/monitor_controller.cpp \
  $(top_builddir)/src/threadconcurrency/posixthread_factory.cpp \
  $(top_builddir)/src/threadconcurrency/mutex_controller.cpp \
  $(top_builddir)/src/taskconcurrency/cliprescan_pe_task.cpp \
  $(top_builddir)/src/taskconcurrency/cliprescan_pe_controller.cpp \
  $(top_builddir)/src/taskconcurrency/tbbpostscan_pe_task.cpp \
  $(top_builddir)/src/taskconcurrency/tbbpostscan_pe_controller.cpp \
  $(top_builddir)/src/taskconcurrency/buffer_sync_tbb.cpp \
  $(top_builddir)/src/taskconcurrency/buffer_sync.cpp \
  $(top_builddir)/src/clibasewrapper/cliwrapper/cli_scanner_wrapper.cpp \
  $(top_builddir)/src/clibasewrapper/cliwrapper/cli_others_common_wrapper.cpp \
  $(top_builddir)/src/tbbscan/data_structure/actire_concurrency.cpp \
  $(top_builddir)/src/tbbscan/data_structure/bloomfliter_concurrency.cpp \
  $(top_builddir)/src/tbbscan/data_structure/filter_policy.cpp \
  $(top_builddir)/src/filestructure/pe_layout_controller.cpp \
  $(top_builddir)/src/filestructure/logger/logging.cpp \
  $(top_builddir)/src/filestructure/logger/stringprintf.cpp \
  $(top_builddir)/src/gensign/clamavsig.cpp \
  $(top_builddir)/src/utils/connector/result_infected.cpp \
  $(top_builddir)/src/internet/client_fresh/updated_sign.cpp \
  $(top_builddir)/src/internet/scan_server/scan_server.cpp \
  $(top_builddir)/src/internet/scan_server/scan_connection.cpp \
  $(top_builddir)/src/internet/security/uuid_controller.cpp \
  $(top_builddir)/src/internet/security/ip_controller.cpp \
  $(top_builddir)/src/internet/security/aes_controller.cpp \
  $(top_builddir)/src/internet/security/encryption_field.cpp \
  $(top_builddir)/src/internet/tracethreat/infected_controller.cpp \
  $(top_builddir)/src/internet/scan_client/scan_client.cpp \
  $(top_builddir)/src/internet/scan_client/basic_scan_dir_service.cpp \
  $(top_builddir)/src/internet/scan_client/basic_scan_dir.cpp \
  $(top_builddir)/src/internet/scan_client/scan_dir_impl.cpp \
  $(top_builddir)/src/internet/msg/packedmessage_scan_client.cpp \
  $(top_builddir)/src/internet/msg/packedmessage_scan.cpp















libthreadsyncocl_a_LIBADD = $(top_builddir)/src/utils/libutils.a \
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
$(top_builddir)/src/internet/msg/libmsg.a $(AM_LDFLAGS)
