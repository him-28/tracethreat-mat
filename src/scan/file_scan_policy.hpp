#ifndef POLICY_FILE_SCAN_POLICY__HPP
#define POLICY_FILE_SCAN_POLICY__HPP

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

}

#endif /* POLICY_FILE_SCAN_POLICY__HPP */
