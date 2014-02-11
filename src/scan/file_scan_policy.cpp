
#include "scan/file_scan_policy.hpp"
#include "scan/pe_file_policy.hpp"

namespace policy
{

    //template<typename MAPPED_FILE>
    //struct file_scan_result;


    template<typename MAPPED_FILE>
    file_controller_policy<MAPPED_FILE>::file_controller_policy()
    {
        //logger
        logger_ptr = &h_util::clutil_logging<std::string, int>::get_instance();
        logger = logger_ptr->get();
        logger->write_info_test("Init logger file_controller_policy");
    }

    template<typename MAPPED_FILE>
    std::vector<struct file_scan_result<MAPPED_FILE>* >& file_controller_policy<MAPPED_FILE>::
    scan_file_engine(file_controller_policy<MAPPED_FILE> *f_col_policy)
    {
        std::vector<MAPPED_FILE *> *mapped_file_vec =  f_col_policy->get_mapped_file();
        typename std::vector<MAPPED_FILE *>::iterator iter_mapped_file;
        uint8_t result_file_count = 0;

        for(iter_mapped_file  = mapped_file_vec->begin();
                iter_mapped_file != mapped_file_vec->end();
                ++iter_mapped_file) {
            MAPPED_FILE *mapp_file  =  *iter_mapped_file;

            if(f_col_policy->scan_file_type(mapp_file)) {
                logger->write_info("Scan found infected file ",
                        boost::lexical_cast<std::string>(mapp_file->file_name));

                //create struct file scan result contain on vector.
                fs_result = new file_scan_result<MAPPED_FILE>();
                file_scan_result_vec.push_back(fs_result);
                file_scan_result_vec[result_file_count]->file_name = mapp_file->file_name;

                //count result size of vector
                result_file_count++;
            }

            // TO-DO
            //f_col_policy->load_plugins_type(mapp_file, pl_result);
        }
    }

    template class file_controller_policy<struct MAPPED_FILE_PE>;

}

