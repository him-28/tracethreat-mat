#ifndef SCAN__PE_POLICY__HPP
#define SCAN__PE_POLICY__HPP

/*						Titles														Authors													Date
 * Scan file by select type with Policy based class
 *                                              R.Chatsiri                      24/01/2014
 */


#include "scan/file_policy.hpp"


template<typename Policy, typename MAPPED_FILE = struct MAPPED_FILE_PE>
class pe_file_policy : public file_controller_policy<MAPPED_FILE>
{

    protected:
			bool scan_file_type(MAPPED_FILE * mapped_file);       
			bool load_plugins_type(MAPPED_FILE * mapped_file);
			file_scan_result<MAPPED_FILE_PE> & get_result()const;
			struct MemoryBlockContext * entry_point_offset(struct MEMORY_BLOCK_PE * memoblock);
    private:
        pe_file_controller<MAPPED_FILE> pe_fcol;

};

#endif /* SCAN__PE_POLICY__HPP */

