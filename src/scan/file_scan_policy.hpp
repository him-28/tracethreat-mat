#ifndef POLICY_FILE_SCAN_POLICY__HPP
#define POLICY_FILE_SCAN_POLICY__HPP

#include <stdlib.h>
#ifndef _MSC_VER
#include "stdint.h"   //Linux standard header.
#else
#include "utils/base" //support VCpp.
#endif 

namespace policy
{

    struct plugins_result {
        uint8_t  status;
        bool     infected_status;
    };

    template<typename MAPPED_FILE>
    struct file_scan_result {
        const char *file_name;
        size_t       file_size;
        MAPPED_FILE  file_detail;

    };

		
		template<typename Base, typename D>
		class disting : public Base{

		}

		template<typename SetterOne, typename SetterTwo>
		class file_policy_selector : public disting<SetterOne,1>
		{


		};

    template<typename MAPPED_FILE>
    class file_policy
    {
        public:
            virtual	std::vector<struct file_scan_result<MAPPED_FILE *> >&
            scan_file_engine(file_controller_policy<MAPPED_FILE> *f_col_policy) = 0;
    };

    template<typename MAPPED_FILE>
    class file_controller_policy : public file_policy<MAPPED_FILE>
    {
        public:
						//Default Policies
            typedef pe_file_policy pe_scan;

            std::vector<struct file_scan_result<MAPPED_FILE *> >&
            scan_file_engine(file_controller_policy<MAPPED_FILE> *f_col_policy);



        protected:
            virtual bool scan_file_type(MAPPED_FILE *mapped_file) = 0;
            virtual bool load_plugins_type(MAPPED_FILE *mapped_file) = 0;
            virtual file_scan_result<MAPPED_FILE>& get_result()const = 0;

        private:
            file_policy<MAPPED_FILE> *f_policy;
            file_scan_result<MAPPED_FILE> *fs_result;
            std::vector<struct file_scan_result<MAPPED_FILE> * >  file_scan_result_vec;
    };

    template<typename MAPPED_FILE> class default_file_policy_args :
        virtual public file_controller_policy<MAPPED_FILE>
    {

    }

		// PE policy mask of pe_file_policy
    template<typename Policy, typename MAPPED_FILE = struct MAPPED_FILE_PE>
    class pe_policy_is : virtual public file_controller_policy<MAPPED_FILE>
    {
        public:
            typedef Policy pe_policy;

    };




    template<typename file_policy_setter_pe = pe_file_policy<MAPPED_FILE> >
    class scan_file_policy : private pe_file_policy<MAPPED_FILE>
    {
        private:
            typedef file_policy_selector<file_policy_setter_pe>  policy;
        public:
            // pe type support
            std::vector<struct file_scan_result<MAPPED_FILE *> >& scan_pe(
                    file_controller_policy<MAPPED_FILE> *f_col_policy) {
                return file_policy_selector::file_policy_controller
												::scan_file_engine(f_col_policy);
            }
    };


}

#endif /* POLICY_FILE_SCAN_POLICY__HPP */
