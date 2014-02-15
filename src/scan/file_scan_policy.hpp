#ifndef POLICY_FILE_SCAN_POLICY_HPP
#define POLICY_FILE_SCAN_POLICY_HPP

#include <vector>

#include "filetypes/pe_file_controller.hpp"
#include "filetypes/pe.hpp"

//logger
#include "utils/logger/clutil_logger.hpp"
#include "utils/base/common.hpp"

namespace policy
{

    namespace h_util = hnmav_util;
    using namespace filetypes;

    template<typename MAPPED_FILE>
    class file_scan_policy;

    template<typename MAPPED_FILE>
    struct file_scan_result;

    template<typename MAPPED_FILE>
    class pe_file_policy;


    struct plugins_result {
        uint8_t  status;
        bool     infected_status;
    };



    //----------------------------  File Scan Policy -------------------------------//
    template<typename MAPPED_FILE>
    struct file_scan_result {
        /**
        * @brief
        */
        unsigned char *file_name;
        /**
        * @brief
        */
        size_t       file_size;
        /**
        * @brief
        */
        //   MAPPED_FILE  file_detail;

    };


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
            //Default Policies
            /**
            * @brief
            */
            typedef pe_file_policy<MAPPED_FILE> pe_scan;

            /**
            * @brief
            */
            file_scan_policy();


            /**
            * @brief
            *
            * @param f_col_policy
            *
            * @return
            */
            std::vector<struct file_scan_result<MAPPED_FILE> * >&
            scan_file_engine(file_scan_policy<MAPPED_FILE> *f_col_policy);


        public:
            //        protected:


            /**
            * @brief
            *
            * @param mapped_file
            *
            * @return
            */
            virtual bool scan_file_type(MAPPED_FILE *mapped_file) = 0;
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
            virtual struct file_scan_result<MAPPED_FILE>& get_result()const = 0;
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

            //virtual ~file_scan_policy();

        private:
            //file_policy<MAPPED_FILE> *f_policy;
            file_scan_result<MAPPED_FILE> *fs_result;
            std::vector<struct file_scan_result<MAPPED_FILE> * >  file_scan_result_vec;

            file_scan_policy<MAPPED_FILE> *f_col_policy;

            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

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

    template<typename MAPPED_FILE,
             typename FilePolicySetter = default_file_policy_args<MAPPED_FILE> >
    class scan_file_policy
    {
        private:
            typedef file_policy_selector<FilePolicySetter>  policy;
        public:
            // pe type support
            std::vector<struct file_scan_result<MAPPED_FILE> * >&
              scan_pe(
                    file_scan_policy<MAPPED_FILE> *obj_fconl_policy) {
                return obj_fconl_policy->scan_file_engine(obj_fconl_policy);
            }
    };



    //-----------------------PE File Policy --------------------------------//
    template<typename MAPPED_FILE>
    class pe_file_policy :  public file_scan_policy<MAPPED_FILE>
    {
        public:

            pe_file_policy();
            ~pe_file_policy();
            //protected:
            /**
            * @brief Add mapped file detail for scanning
            *
            * @param mapped_file Mapped file included offset and size of offset
            *
            * @return True, If scanning completed.
            */
            // virtual bool scan_file_type(MAPPED_FILE *mapped_file);
            // : Cannot use virtual from file_scan_policy abstract base
            virtual bool scan_file_type(MAPPED_FILE *mapped_file);

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
            virtual struct file_scan_result<MAPPED_FILE>& get_result()const;
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
            * @return
            */
            virtual std::vector<MAPPED_FILE *> *get_mapped_file();
            /**
            * @brief
            *
            * @param mapped_file
            *
            * @return
            */
            virtual bool set_mapped_file(MAPPED_FILE *mapped_file);
        private:
            pe_file_controller<MAPPED_FILE> pe_fconl;
            // mapped_file detail
            std::vector<MAPPED_FILE * > mapped_files_vec;

            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };

}

#endif /* POLICY_FILE_SCAN_POLICY__HPP */
