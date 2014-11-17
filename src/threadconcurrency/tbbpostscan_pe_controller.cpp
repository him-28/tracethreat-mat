#include <utility>
#include "boost/functional/hash.hpp"
#include "threadconcurrency/tbbpostscan_pe_controller.hpp"

namespace controller
{
    template<typename BufferSync, typename MAPPED_FILE, typename SignatureTypeMemory>
    bool tbbpostscan_pe_controller<BufferSync, MAPPED_FILE, SignatureTypeMemory>::init_syntbb_workload(
            typename shm_memory::file_shm_handler<MAPPED_FILE>::map_str_shm& mapstr_shm,
            signature_shm_type *sig_shm,
            std::map<const uint64_t , size_t> *map_file_size,
            std::vector<MAPPED_FILE *> *mapped_file_vec)
    {

        logger->write_info("tbbpostscan_pe_controller::init_syntbb_workload, Start Load data to task",
                hnmav_util::format_type::type_center);

        std::vector<std::string>  sig_key_vec;
        //struct tbbscan::results_callback<std::vector<std::string> >  res_callback(sig_key_vec);
        //typename tbbpostscan_pe_task<MAPPED_FILE, SignatureTypeMemory>::res_callback_type res_callback;
        //[x] Get all data from mapped_file_vec for invoke file_name in order hash data.
        //[x]  Insert binary from File-SHM to write_binary_hex is contains all binary hex file.

        //new BufferSyncTBB();
        buff_sync_internal  = new BufferSync();
        boost::hash<char *>  hash_file_name;

        typename shm_memory::file_shm_handler<MAPPED_FILE>::binary_string_shm    *binarystr_shm;
        typename shm_memory::file_shm_handler<MAPPED_FILE>::map_str_shm::iterator iter_mapstr_shm;
        typename std::vector<MAPPED_FILE *>::iterator iter_mapped_file_vec;

        //Loop initial vector : binary_hex
        for(iter_mapped_file_vec = mapped_file_vec->begin();
                iter_mapped_file_vec != mapped_file_vec->end();
                ++iter_mapped_file_vec) {
            MAPPED_FILE   *mapped_file = *iter_mapped_file_vec;
            //changed file name to md5 hash.
            const uint64_t file_name_md5  =
                    hash_file_name(const_cast<char *>(mapped_file->file_name.c_str()));

            logger->write_info("tbbpostscan_pe_controller::init_syntbb_workload, File-Name",
                    boost::lexical_cast<std::string>(mapped_file->file_name));
            logger->write_info("tbbpostscan_pe_controller::init_syntbb_workload, MD5 of file",
                    boost::lexical_cast<std::string>(file_name_md5));

            //get File-SHM from file_name_md5
            iter_mapstr_shm  = mapstr_shm.find(file_name_md5);
            //Mapp File-SHM contain Key: file_name_md5, Value : Binary stream.
            typename shm_memory::file_shm_handler<MAPPED_FILE>::
            value_types_str  pair_int_str = *iter_mapstr_shm;

            //binary stream from file.
            binarystr_shm = &pair_int_str.second;

            //register file: md5
            if(buff_sync_internal->filemd5_regis_tbb(file_name_md5, mapped_file->file_name.c_str()))
                logger->write_info("tbbpostscan_pe_controller::init_syntbb_workload, Map not completed.");

            //Get map : Key : md5, Value : Size of file.
            std::pair<const uint64_t, size_t> pair_fsize =  *map_file_size->find(file_name_md5);

            size_t size_hex = pair_fsize.second;

            //Add binary stream to vector_concurrent
            if(buff_sync_internal->write_binary_hex_tbb(binarystr_shm->c_str(),
                    size_hex,
                    file_name_md5)) {

            }//if	

						
						/*//Check bug parallel insert
						for(int count_str = 0; count_str < buff_sync_internal->buff->binary_hex.size(); count_str++)
						{
							std::cout<<"Test input : "<< buff_sync_internal->buff->binary_hex[count_str]<<std::endl;				
						}
						*/

        }//for

        //[x] Get Position per file add to task search.
        //[x] Object actire_concurrency add to task search.
        //[x] Task set contains on set.
        //[x] Insert start-end point scan on binary_hex vector_concurrent.
        std::map<uint64_t, MAPPED_FILE *>   *map_fmd5_id =
                &buff_sync_internal->buff->data_tbb_process<MAPPED_FILE>::fmd5_tbb_map;

        logger->write_info_test("tbbpostscan_pe_controller::init_syntbb_workload, mapped_file size",
                boost::lexical_cast<std::string>(map_fmd5_id->size()));

        typename data_tbb_process<MAPPED_FILE>::fmd5_map_type::iterator iter_mapfmd5;

        for(iter_mapfmd5 = map_fmd5_id->begin();
                iter_mapfmd5 != map_fmd5_id->end();
                ++iter_mapfmd5) {

            logger->write_info("Start prepair data");

            std::pair<uint64_t, MAPPED_FILE *> pair_s_tbb = *iter_mapfmd5;
            MAPPED_FILE *s_tbb = pair_s_tbb.second;


            uint64_t start_point = s_tbb->start_point;
            uint64_t end_point   = s_tbb->end_point;


            std::vector<std::string> data_sig;
            tbbscan::result_callback<std::vector<std::string> >  res_callback(data_sig);
						task_count   = map_fmd5_id->size(); // Size of map contain number of file per task.
						worker_count = map_fmd5_id->size(); // worker[ task-1, task-2, task-3..]. tasks per work.
						timeout_     = 1000LL; //default
            tbbscan_pe_task =
                    new tbbpostscan_pe_task(monitor, task_count, timeout_);

            tbbscan_pe_task->set_file(&buff_sync_internal->buff->binary_hex);//Binary stream

            tbbscan_pe_task->set_file_name(s_tbb->file_name.c_str()); // file name.
            tbbscan_pe_task->set_point(start_point, end_point); //position on binary stream for scanning.
            tbbscan_pe_task->set_callback(&res_callback);
            tbbscan_pe_task->set_sig_engine(actire_engine_);
            tbbscan_pe_task->set_search_engine(iactire_concur_);
						/*
						printf("\nCheck Position\n");
						tbb::concurrent_vector<char> test_vec = buff_sync_internal->buff->binary_hex;
						for(int count_start = start_point; count_start < 60; count_start++)
							printf("%c", test_vec[count_start]);
						printf("\nEnd Position\n");
						*/
            //insert to tasks per thread.
            tasks_tbbscan_pe.insert(boost::shared_ptr<tbbpostscan_pe_task_type>(tbbscan_pe_task));

            logger->write_info("tbbpostscan_pe_controller::init_syntbb_workload, Insert file to task",
                    boost::lexical_cast<std::string>(s_tbb->file_name));

            logger->write_info("tbbpostscan_pe_controller::init_syntbb_workload, MD5",
                    boost::lexical_cast<std::string>(pair_s_tbb.first));

            logger->write_info("tbbpostscan_pe_controller::init_syntbb_workload, Scan start point",
                    boost::lexical_cast<std::string>(start_point));

            logger->write_info("tbbpostscan_pe_controller::init_syntbb_workload, End start point",
                    boost::lexical_cast<std::string>(end_point));

            logger->write_info("tbbpostscan_pe_controller::init_syntbb_workload, File stream size",
                    boost::lexical_cast<std::string>(buff_sync_internal->buff->binary_hex.size()));

                    logger->write_info("End Prepair data");


        }//for

                logger->write_info("tbbpostscan_pe_controller::init_syntbb_workload, End Load data to task",
                        hnmav_util::format_type::type_center);
				return true;
    }

    template<typename BufferSync, typename MAPPED_FILE, typename SignatureTypeMemory>
    bool tbbpostscan_pe_controller<BufferSync, MAPPED_FILE, SignatureTypeMemory>::
    add_sig_engine(actire_sig_engine_type *_actire_engine)
    {
        actire_engine_ = _actire_engine;
				return true;
    }

    template<typename BufferSync, typename MAPPED_FILE, typename SignatureTypeMemory>
    bool tbbpostscan_pe_controller<BufferSync, MAPPED_FILE, SignatureTypeMemory>::
    add_search_engine(iactire_concur_type   *_iactire_concur)
    {
        iactire_concur_ = _iactire_concur;
				return true;
    }

    template<typename BufferSync, typename MAPPED_FILE, typename SignatureTypeMemory>
    bool tbbpostscan_pe_controller<BufferSync, MAPPED_FILE, SignatureTypeMemory>::
    task_start()
    {
        logger->write_info("tbbpostscan_pe_controller::task_start, Start Task",
                hnmav_util::format_type::type_center);

				logger->write_info("tbbpostscan_pe_controller::task_start, Task size",
							 boost::lexical_cast<std::string>(tasks_tbbscan_pe.size()));

        boost::shared_ptr<thread_manager>  thread_m =
                thread_manager::new_simple_thread_manager(worker_count);

        thread_m->thread_factory(boost::shared_ptr<platformthread_factory>(
                new platformthread_factory()));

        thread_m->start();

				typename std::set<boost::shared_ptr<tbbpostscan_pe_task> >::iterator ix;
        for(ix = tasks_tbbscan_pe.begin();
                ix != tasks_tbbscan_pe.end();
                ++ix) {

            thread_m->add(*ix);
					  tbbpostscan_pe_task * pe_scan_task = ix->get();
						threatinfo_ptr = pe_scan_task->get_threatinfo();
        }//for

        {

            synchronized s(monitor);

            while(task_count) {
                monitor.wait();
            }
        }//sync

			logger->write_info("tbbpostscan_pe_controller::task_start, Completed task");

			return true;
    }

    template class tbbpostscan_pe_controller<BufferSyncTBB<struct data_tbb_process<struct MAPPED_FILE_PE>,
             struct MAPPED_FILE_PE>,
             struct MAPPED_FILE_PE,
             struct utils::meta_sig>;



}
