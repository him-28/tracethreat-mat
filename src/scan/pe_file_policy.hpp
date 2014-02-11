#ifndef POLICY_SCAN__PE_POLICY__HPP
#define POLICY_SCAN__PE_POLICY__HPP

/*						Titles														Authors													Date
 * Scan file by select type with Policy based class
 *                                              R.Chatsiri                      24/01/2014
 */

#include "boost/shared_ptr.hpp"

#include "scan/file_scan_policy.hpp"
#include "filetypes/pe_file_controller.hpp"

#include "utils/logger/clutil_logger.hpp"

namespace policy
{

    using namespace filetypes;

    namespace h_util = hnmav_util;

    // Forward declaration.
    template<typename MAPPED_FILE>
    class file_controller_policy;

    template<typename MAPPED_FILE>
    class file_scan_result;

    template<typename MAPPED_FILE>
    class pe_file_policy :  public file_controller_policy<MAPPED_FILE>
    {

        protected:
            pe_file_policy();
            /**
            * @brief Add mapped file detail for scanning
            *
            * @param mapped_file Mapped file included offset and size of offset
            *
            * @return True, If scanning completed.
            */
            bool scan_file_type(MAPPED_FILE *mapped_file);
            /**
            * @brief
            *
            * @param mapped_file
            *
            * @return
            */
            bool load_plugins_type(MAPPED_FILE *mapped_file);
            /**
            * @brief
            *
            * @return
            */
            file_scan_result<struct MAPPED_FILE_PE>& get_result()const;
            /**
            * @brief
            *
            * @param memoblock
            *
            * @return
            */
            struct MemoryBlockContext *entry_point_offset(struct MEMORY_BLOCK_PE *memoblock);
            /**
            * @brief
            *
            * @return
            */
            std::vector<MAPPED_FILE *> *get_mapped_file();
            /**
            * @brief
            *
            * @param mapped_file
            *
            * @return
            */
            bool set_mapped_file(MAPPED_FILE *mapped_file);
        private:
            pe_file_controller<MAPPED_FILE> pe_fconl;
            // mapped_file detail
            std::vector<MAPPED_FILE * > mapped_files_vec;

            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };


}


#endif /* SCAN__PE_POLICY__HPP */

