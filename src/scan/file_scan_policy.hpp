#ifndef POLICY_FILE_SCAN_POLICY_HPP
#define POLICY_FILE_SCAN_POLICY_HPP

#include <vector>

#include "filetypes/pe_file_controller.hpp"
#include "filetypes/pe.hpp"

#include "memory/signature_shm_base.hpp"
#include "memory/signature_shm_controller.hpp"

#include "utils/file_offset_handler.hpp"
#include "tbbscan/data_structure/actire_concurrency.hpp"
#include "filestructure/pe_layout_controller.hpp"

#include "msg/message_tracethreat.pb.h"

//logger
#include "utils/logger/clutil_logger.hpp"
#include "utils/base/common.hpp"

namespace policy
{

    namespace h_util = hnmav_util;
    namespace ftypes = filetypes;
    //using namespace filetypes;
    using utils::file_scan_result;
    using filestructure::pe_layout_controller;

    template<typename MAPPED_FILE>
    class file_scan_policy;
    /*
    template<typename MAPPED_FILE>
    struct file_scan_result;
    */
    template<typename MAPPED_FILE>
    class pe_file_policy;


    //____________________________________  File Scan Policy ___________________________________//

    template<typename Base, int D>
    class disting : public Base
    {

    };

    template<typename Setter1>
    class file_policy_selector : public disting<Setter1, 1>
    {


    };

    /*
    template<typename MAPPED_FILE>
    class file_policy
    {
    public:
        virtual	std::vector<struct file_scan_result<MAPPED_FILE > * >& \
        scan_file_engine(file_scan_policy<MAPPED_FILE> *f_col_policy) = 0;
    };
    */

    template<typename MAPPED_FILE>
    class file_scan_policy// : public file_policy<MAPPED_FILE>
    {
        public:

            typedef memory::signature_shm<struct memory::meta_sig, struct memory::meta_sig_mem>
                        sig_shm_type;

            typedef tbbscan::actire_sig_engine<char, tbbscan::tbb_allocator>  sig_engine_type;

            typedef tbbscan::iactire_engine<char, tbbscan::tbb_allocator>
            iactire_engine_scanner_type;

						typedef boost::shared_ptr<message_tracethreat::InfectedFileInfo>  threatinfo_ptr_type;

            /**
            * @brief
            */
            typedef pe_file_policy<MAPPED_FILE> pe_scan;

            /**
            * @brief
            */
            file_scan_policy();


            //std::vector<struct utils::file_scan_result<MAPPED_FILE>* >&
            threatinfo_ptr_type
            scan_file_engine(file_scan_policy<MAPPED_FILE> *fcol_policy,
                    sig_shm_type   *sig_shm_pe,
                    sig_engine_type *sig_engine,
                    iactire_engine_scanner_type *iactire_engine_scanner);

            //std::vector<struct utils::file_scan_result<MAPPED_FILE>* >&
            threatinfo_ptr_type
            scan_file_engine(file_scan_policy<MAPPED_FILE> *fcol_policy,
                    std::vector<MAPPED_FILE *> *mapped_file_vec,
                    memory::signature_shm<struct memory::meta_sig,
                    struct memory::meta_sig_mem> *sig_shm);

            template<typename SymbolT, typename StateT>
            std::vector<struct utils::file_scan_result<MAPPED_FILE> * >&
            set_sig_buffer(std::vector<SymbolT> *node_symbol,
                    std::vector<StateT> *node_state) {
                node_symbol_vec = node_symbol;
                node_state_vec  = node_state;
            }

            template<typename SymbolT, typename StateT>
            std::vector<struct utils::file_scan_result<MAPPED_FILE> * >&
            receive_signature(boost::shared_ptr<std::vector<SymbolT> >& symbol_shared_ptr,
                    boost::shared_ptr<std::vector<StateT> >& state_shared_ptr) {
                //TODO:
            }

            /**
            * @brief Set Kernel file of scanning on GPGPU system.
            *
            * @param kernel_file_path  Kernel file extension name .cl
            *
            * @return True, If file contains strings more size than zero.
            */
            bool set_opencl_file_path(std::string& kernel_file_path) {

                if(kernel_file_path.size() == 0) return false;

                this->kernel_file_path = &kernel_file_path;
                return true;
            }


        public:

            /**
            * @brief
            *
            * @param mapped_file
            *
            * @return
            */
            virtual threatinfo_ptr_type 
										scan_file_type(std::vector<const char *> *file_type_vec,
                    std::vector<MAPPED_FILE *> *mapped_file_pe,
                    sig_shm_type  *sig_shm,
                    sig_engine_type *sig_engine,
                    iactire_engine_scanner_type   *iactire_engine_scanner) = 0;


            /**
            * @brief
            *
            * @param mapped_file
            *
            * @return
            */
            virtual threatinfo_ptr_type
									  scan_file_type(std::vector<MAPPED_FILE *> *mapped_file,
                    memory::signature_shm<struct memory::meta_sig,
                    struct memory::meta_sig_mem> * sig_shm) = 0;
            /**
            * @brief
            *
            * @param mapped_file
            *
            * @return
            */
            //  Class didn't support virtual, Abstract base
            virtual bool load_plugins_type(MAPPED_FILE *mapped_file) = 0;
            /**
            * @brief
            *
            * @return
            */
            virtual struct utils::file_scan_result<MAPPED_FILE>& get_result()const = 0;
            /**
            * @brief
            *
            * @return
            */
            virtual std::vector<MAPPED_FILE *> *get_mapped_file() = 0;
            /**
            * @brief
            *
            * @param mapped_file
            *
            * @return
            */
            virtual bool set_mapped_file(MAPPED_FILE *mapped_file) = 0;


            /**
            * @brief
            *
            * @param mapped_file
            *
            * @return
            */
            virtual bool set_mapped_file(std::vector<MAPPED_FILE *>  *mapped_file) = 0;

            /**
            * @brief
            *
            * @param file_type_vec
            *
            * @return
            */
            virtual bool set_file_type(std::vector<const char *>    *file_type_vec) = 0;

            /**
            * @brief
            *
            * @return
            */
            virtual std::vector<const char *> *get_file_type() = 0;

        private:
            utils::file_scan_result<MAPPED_FILE> *fs_result;
            std::vector<struct utils::file_scan_result<MAPPED_FILE> * >  file_scan_result_vec;

            file_scan_policy<MAPPED_FILE> *f_col_policy;

            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

        protected :
            std::vector<char> *node_symbol_vec;

            std::vector<size_t> *node_state_vec;

            std::string *kernel_file_path;

    };

    template<typename MAPPED_FILE> class default_file_policy_args :
        virtual public file_scan_policy<MAPPED_FILE>
    {

    };

    // PE policy mask of pe_file_policy
    template<typename Policy, typename MAPPED_FILE = struct MAPPED_FILE_PE>
    class pe_policy_is : virtual public file_scan_policy<MAPPED_FILE>
    {
        public:
            typedef Policy pe_policy;

    };


    //_________________________________  Scan File Policy __________________________________//
    template<typename MAPPED_FILE,
             typename FilePolicySetter = default_file_policy_args<MAPPED_FILE> >
    class scan_file_policy
    {
        private:
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

            typedef file_policy_selector<FilePolicySetter>  policy;


        public:

            typedef memory::signature_shm<struct memory::meta_sig, struct memory::meta_sig_mem>
                        sig_shm_type;

            typedef tbbscan::actire_sig_engine<char, tbbscan::tbb_allocator>  sig_engine_type;

            typedef tbbscan::iactire_engine<char, tbbscan::tbb_allocator>
            iactire_engine_scanner_type;

						typedef boost::shared_ptr<message_tracethreat::InfectedFileInfo>  threatinfo_ptr_type;


            // pe type support
            //std::vector<struct utils::file_scan_result<MAPPED_FILE> * >&
            threatinfo_ptr_type
            scan_pe(file_scan_policy<MAPPED_FILE> *obj_fconl_policy,
                    sig_shm_type *sig_shm,
                    sig_engine_type *sig_engine,
                    iactire_engine_scanner_type *iactire_engine_scanner) {

                //TODO: test only
                //OCL Mode :  utils::scanning_mode smode = utils::multiple_ocl_mode;
								 utils::scanning_mode smode = utils::multiple_tbb_mode;
                //Policy multiple scanning file with-OCL
                switch(smode) {

                case utils::multiple_ocl_mode : { //multiple scanning on OCL
                    logger->write_info("scan_file_policy::scan_pe(), Mode : multiple_ocl_mode");
                    //get data, size mapped_file for API system.
                    std::vector<MAPPED_FILE *> *mapped_file_vec = obj_fconl_policy->get_mapped_file();
                    //Send ot Multiple file OCL mode.
                    return obj_fconl_policy->scan_file_engine(obj_fconl_policy,
                            mapped_file_vec,
                            sig_shm);
                }

                case utils::multiple_tbb_mode : { //multiple scanning on TBB
                    logger->write_info("scan_file_policy::scan_pe(), Mode : multiple_tbb_mode");

                    return obj_fconl_policy->scan_file_engine(obj_fconl_policy,
                            sig_shm,
                            sig_engine,
                            iactire_engine_scanner);
                }

                case utils::multiple_ocl_tbb_mode : { //Priority OCL before TBB mode.
                    logger->write_info("scan_file_policy::scan_pe(), Mode : multiple_ocl_tbb_mode");

                    //return obj_fconl_policy->scan_file_engine(obj_fconl_policy);
                }

                }


            }
    };



    //__________________________________ PE File Policy _________________________________________//
    template<typename MAPPED_FILE>
    class pe_file_policy :  public file_scan_policy<MAPPED_FILE>
    {
        public:

            typedef memory::signature_shm<struct memory::meta_sig, struct memory::meta_sig_mem>
                        sig_shm_type;

            typedef tbbscan::actire_sig_engine<char, tbbscan::tbb_allocator>
            sig_engine_type;

            typedef tbbscan::iactire_engine<char, tbbscan::tbb_allocator>
            iactire_engine_scanner_type;

            typedef utils::file_offset_handler<struct utils::common_filetype, struct MAPPED_FILE_PE>
                        fileoffset_type;

            typedef filestructure::pe_layout_controller<struct IMAGE_NT_HEADERS_EXT, struct MAPPED_FILE_PE>
                        pe_layout_controller_type;

            typedef controller::BufferSync< struct controller::data_ocl_process<MAPPED_FILE>, MAPPED_FILE>
                    buffer_sync;

						typedef boost::shared_ptr<message_tracethreat::InfectedFileInfo>  threatinfo_ptr_type;

            pe_file_policy();

            ~pe_file_policy();

            virtual threatinfo_ptr_type scan_file_type(std::vector<const char *> *file_type_vec,
                    std::vector<MAPPED_FILE *> *mapped_file_pe,
                    sig_shm_type  *sig_shm,
                    sig_engine_type *sig_engine,
                    iactire_engine_scanner_type   *iactire_engine_scanner);


            virtual threatinfo_ptr_type scan_file_type(std::vector<MAPPED_FILE *> *mapped_file,
                    memory::signature_shm<struct memory::meta_sig,
                    struct memory::meta_sig_mem> * sig_shm);

            /**
            * @brief
            *
            * @param mapped_file
            *
            can_file_type @return
            */
            virtual bool load_plugins_type(MAPPED_FILE *mapped_file);
            /**
            * @brief
            *
            * @return
            */
    virtual struct utils::file_scan_result<MAPPED_FILE>& get_result()const;
            /**
            * @brief
            *
            * @param memoblock
            *
            * @return
            */
            virtual struct MemoryBlockContext *entry_point_offset(struct MEMORY_BLOCK_PE *memoblock);
            /**
            * @brief
            *
            * @param mapped_file
            *
            * @return
            */
            virtual bool set_mapped_file(MAPPED_FILE *mapped_file);

            virtual std::vector<MAPPED_FILE *> *get_mapped_file();

            /**
            * @brief
            *
            * @param mapped_file
            *
            * @return
            */
            virtual bool set_mapped_file(std::vector<MAPPED_FILE *> *mapped_file);


            virtual bool set_file_type(std::vector<const char *> *file_type);

            std::vector<const char *> *get_file_type();

            //data_structure::iparallel<SymbolT, StateT> *ipara
            template<typename SymbolT, typename StateT>
            std::vector<struct utils::file_scan_result<MAPPED_FILE> * >&
            set_sig_buffer(std::vector<SymbolT> node_symbol, std::vector<StateT> node_state) {
								logger->write_info("!!!Error set : set_sig_buffer");
            }



        private:
            ftypes::pe_file_controller<MAPPED_FILE> pe_fconl;
            // mapped_file detail
            std::vector<MAPPED_FILE * > mapped_files_vec;

            std::vector<const char *>    file_type_vec;

            fileoffset_type  fileoffset_h;

            pe_layout_controller_type pe_layout;
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };

}

#endif /* POLICY_FILE_SCAN_POLICY__HPP */
