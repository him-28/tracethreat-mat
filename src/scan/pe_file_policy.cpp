/* 						Titles														Authors 												Date
 * Pe file scanning detected malware with get offset scans signature.
 *                                               R.Chatsiri                     24/01/2014
 */

#include "scan/pe_file_policy.hpp"

template<typename Policy,typename MAPPED_FILE>
bool pe_file_policy<Policy, MAPPED_FILE>::
scan_file_engine(MAPPED_FILE *mapped_file_pe)
{



}

template<typename Policy,typename MAPPED_FILE>
std::vector<struct file_scan_result<MAPPED_FILE> > pe_file_policy<Policy, MAPPED_FILE>::
scan_file_engine(std::vector<MAPPED_FILE *> mapped_file_pe)
{
		typename std::vector<MAPPED_FILE*>::iterator iter_mapped_file_pe;
		

}

