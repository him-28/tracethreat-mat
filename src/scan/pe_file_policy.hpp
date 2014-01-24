#ifndef SCAN__PE_POLICY__HPP
#define SCAN__PE_POLICY__HPP

/*						Titles														Authors													Date
 * Scan file by select type with Policy based class
 *                                              R.Chatsiri                      24/01/2014
 */


#include "scan/file_policy.hpp"


template<typename Policy, typename MAPPED_FILE = struct MAPPED_FILE_PE>
class pe_file_policy : public file_policy<MAPPED_FILE>
{

    protected:
        bool scan_file(MAPPED_FILE *mapped_file_pe);
        std::vector<struct file_scan_result<MAPPED_FILE> >
        scan_file_engine(std::vector<MAPPED_FILE *> mapped_file_pe);
    private:
        pe_file_controller<MAPPED_FILE> pe_type;

};

#endif /* SCAN__PE_POLICY__HPP */

