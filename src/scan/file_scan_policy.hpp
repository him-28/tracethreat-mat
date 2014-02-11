#ifndef POLICY_FILE_SCAN_POLICY__HPP
#define POLICY_FILE_SCAN_POLICY__HPP

#include <vector>

// PE file policy supported
#include "scan/pe_file_policy.hpp"

//logger
#include "utils/logger/clutil_logger.hpp"
#include "utils/base/common.hpp"

namespace policy
{

    namespace h_util = hnmav_util;

    template<typename MAPPED_FILE>
    class file_controller_policy;

    template<typename MAPPED_FILE>
    struct file_scan_result;

    template<typename MAPPED_FILE>
    class pe_file_policy;

    struct plugins_result {
        uint8_t  status;
        bool     infected_status;
    };

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

    template<typename MAPPED_FILE>
    class file_policy
    {
        public:
            virtual	std::vector<struct file_scan_result<MAPPED_FILE > * >& \
            scan_file_engine(file_controller_policy<MAPPED_FILE> *f_col_policy) = 0;
    };

    template<typename MAPPED_FILE>
    class file_controller_policy : public file_policy<MAPPED_FILE>
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
            file_controller_policy();

            /**
            * @brief
            *
            * @param f_col_policy
            *
            * @return
            */
            std::vector<struct file_scan_result<MAPPED_FILE> * >&
            scan_file_engine(file_controller_policy<MAPPED_FILE> *f_col_policy);



        protected:
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
            virtual bool load_plugins_type(MAPPED_FILE *mapped_file) = 0;
            /**
            * @brief
            *
            * @return
            */
            virtual file_scan_result<MAPPED_FILE>& get_result()const = 0;
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

        private:
            file_policy<MAPPED_FILE> *f_policy;
            file_scan_result<MAPPED_FILE> *fs_result;
            std::vector<struct file_scan_result<MAPPED_FILE> * >  file_scan_result_vec;

            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };

    template<typename MAPPED_FILE> class default_file_policy_args :
        virtual public file_controller_policy<MAPPED_FILE>
    {

    };

    // PE policy mask of pe_file_policy
    template<typename Policy, typename MAPPED_FILE = struct MAPPED_FILE_PE>
    class pe_policy_is : virtual public file_controller_policy<MAPPED_FILE>
    {
        public:
            typedef Policy pe_policy;

    };

    template<typename MAPPED_FILE, 
			typename FilePolicySetterPE = pe_file_policy<MAPPED_FILE> >
    class scan_file_policy
    {
        private:
            typedef file_policy_selector<FilePolicySetterPE>  policy;
        public:
            // pe type support
            std::vector<struct file_scan_result<MAPPED_FILE> * >& scan_pe(
                    file_controller_policy<MAPPED_FILE> *f_col_policy) {

            }
    };


}

#endif /* POLICY_FILE_SCAN_POLICY__HPP */
