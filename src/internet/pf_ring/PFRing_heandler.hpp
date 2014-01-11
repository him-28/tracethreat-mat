#ifndef PFRING_HANDLER__HPP
#define PFRING_HANDLER__HPP


/*                       Titles                                          Authors                        Date
 *- PF-RING for internet scans packets over internet                     R.Chatsiri                     11/01/2014
 */


extern "C"{
#include "pcap-int.h" // support PF-RING
}


class PFRing{
public:

inline int add_filtering_rule(filtering_rule  * rule);

private:
pfring * ring;
char * device_name;

};

inline int RFRing::add_filtering_rule(filtering_rule * rule)
{


}


#endif /*PRRING_HANDLER_HPP */
