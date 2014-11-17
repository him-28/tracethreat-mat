#ifndef CONTROLLER_TBBPOSTSCAN_PE_CONTROLLER_HPP
#define CONTROLLER_TBBPOSTSCAN_PE_CONTROLLER_HPP

#include <set>

#include "tbbscan/data_structure/actire_concurrency.hpp"

#include "memory/file_shm_handler.hpp"
#include "memory/signature_shm_controller.hpp"

#include "threadconcurrency/buffer_sync_tbb.hpp"

#include "threadconcurrency/monitor_controller.hpp"

#include "threadconcurrency/tbbpostscan_pe_task.hpp"

#include "msg/message_tracethreat.pb.h"

namespace controller
{

    namespace shm_memory = memory;
    namespace h_util = hnmav_util;

    template<typename BufferSync, typename MAPPED_FILE, typename SignatureTypeMemory>
    class tbbpostscan_pe_controller
    {

        public:

            boost::shared_ptr<BufferSync> buffer_sync;

            typedef BufferSync      buffer_sync_type;

            typename BufferSync::buffer_internal   *buff;


            typedef memory::signature_shm<struct memory::meta_sig, struct memory::meta_sig_mem>
                        signature_shm_type;

            typedef tbbscan::actire_sig_engine<char, true>  actire_sig_engine_type;

            typedef tbbscan::iactire_engine<char, true> iactire_concur_type;

					  typedef tbbpostscan_pe_task tbbpostscan_pe_task_type;


						typedef boost::shared_ptr<message_tracethreat::InfectedFileInfo>  threatinfo_ptr_type;
            //File structure for scanning.
            //Files insert to shm, thus all file contains on vector file-shm( Key : md5 of file.)
            //Mapped file size (Keys are  md5 file of files, value : Sizes of files)
            //Signature use map signature file. (Support SIG-SHM: Plan-00004 )
            bool init_syntbb_workload(
                    typename shm_memory::file_shm_handler<MAPPED_FILE>::map_str_shm& mapstr_shm,
                    signature_shm_type   *sig_shm,
                    std::map<const uint64_t , size_t> *map_file_size,
                    std::vector<MAPPED_FILE *>         *mapped_file_vec);

            //add signature engine.
            bool add_sig_engine(actire_sig_engine_type *_actire_engine);

            //add search engine. Searching from signature engine.
            bool add_search_engine(iactire_concur_type    *_iactire_concur);

            //run multiple file scanning this member function.
            bool task_start();

						threatinfo_ptr_type get_threatinfo(){ return threatinfo_ptr; }

        private:
            //pe engine scanning
            actire_sig_engine_type   *actire_engine_;
            iactire_concur_type *iactire_concur_;

            buffer_sync_type *buff_sync_internal;

            size_t worker_count;
            size_t task_count;

            int64_t  timeout_;

            std::set<boost::shared_ptr<tbbpostscan_pe_task> >
            tasks_tbbscan_pe;

            tbbpostscan_pe_task *tbbscan_pe_task;
            monitor_controller  monitor;

						threatinfo_ptr_type  threatinfo_ptr;
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;
    };

}





#endif  /* CONTROLLER_TBBPOSTSCAN_PE_CONTROLLER_HPP */
