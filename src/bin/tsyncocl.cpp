#include "utils/file_offset_handler.hpp"
#include "scan/file_scan_policy.hpp"
#include "data_structure/actire_parallel.hpp"
#include "data_structure/ac_tire.hpp"
#include "CL/cl.h"
#define FILE_ON 1



using namespace data_structure;
using namespace policy;
using namespace util;

typedef struct {
    std::vector<std::string> keywords;
    char const *input;
} test_data;

int main()
{
/*
    //load binary file pe.

    char const *file_name_offset[FILE_ON];

    file_name_offset[0] = "/home/chatsiri/sda1/workspacecpp/clamav-devel/test/Crack.exe";
    opencl_file_path    = "/home/chatsiri/workspacecpp/pthread_sync_ocl/src/ocl/cl";

    for(int count_file = 0; count_file < 	FILE_ON; count_file++) {
        list_file_type.push_back(file_name_offset[count_file]);

        s_mapped_fpe[count_file]	 = (struct MAPPED_FILE_PE *)malloc(sizeof(s_mapped_fpe));

        mapped_file_vec.push_back(s_mapped_fpe[count_file]);

    }

    //load signature and input file
    td[0].keywords = boost::assign::list_of("4bc647")("4b537db2e3a337")("4b53abf");
    td[0].input    = "a824bc647c46d04b537db2e3a33711";


    std::list<std::string> list_file_type;
    struct MAPPED_FILE_PE *s_mapped_fpe[FILE_ON];
    std::vector<MAPPED_FILE_PE *> mapped_file_vec;

    //test with actire_parallel
    typedef boost::unordered_map<char, size_t>  unordered_map;
    actire_parallel<char, size_t, unordered_map, std::vector<unordered_map> > acp;
    std::string  opencl_file_path;
    test_data td[1];



    for(int i = 0; i  < 1; i++) {
        char *input_tmp = const_cast<char *>(&td[i].input[0]);
        char *end_input_tmp = const_cast<char *>(&td[i].input[strlen(td[i].input)]);
        ac_graph<char> ac(td[i].keywords.begin(),td[i].keywords.end());
        acp.prepare_graph(&ac.get_graph());
    }

    std::vector<char>   nsymbol;
    std::vector<size_t> nstate;
    boost::shared_ptr<std::vector<char> > symbol_shared_ptr  = acp.get_symbol_shared_ptr();
    boost::shared_ptr<std::vector<size_t> > state_shared_ptr = acp.get_state_shared_ptr();
    // Second, Send Symbol and State vector to  file_scan_policy
    // list_file_tye insert file name, s_mapped_fpe inserted  file_type details.
    file_offset_handler<struct common_filetype, struct MAPPED_FILE_PE>  fileoffset_h;
    fileoffset_h.mapped_file(list_file_type, mapped_file_vec, fileoffset_h);

    std::vector<struct MAPPED_FILE_PE *> mapf_vec = fileoffset_h.get_mapped_file();
    typename std::vector<struct MAPPED_FILE_PE *>::iterator iter_mapf_vec;
    MAPPED_FILE_PE *mf_pe;

    for(iter_mapf_vec = mapf_vec.begin(); iter_mapf_vec != mapf_vec.end(); ++iter_mapf_vec) {
        mf_pe = *iter_mapf_vec;
        size_t data_size = mf_pe->size;
        EXPECT_GT(data_size,0);
    }

    //Scan step
    file_scan_policy<struct MAPPED_FILE_PE> *pef_policy = new pe_file_policy<struct MAPPED_FILE_PE>();
    // Scan_ocl_controller member function send vector symbol.
    pef_policy->scan_ocl_controller(symbol_shared_ptr.get(), state_shared_ptr.get());
    pef_policy->set_opencl_file_path(opencl_file_path);
    pef_policy->set_mapped_file(mf_pe);
    scan_file_policy<
    MAPPED_FILE_PE,
    pe_policy_is<pe_file_policy<MAPPED_FILE_PE>, MAPPED_FILE_PE>
    > sf_policy;
    sf_policy.scan_pe(pef_policy);

    fileoffset_h.unmapped_file(mapped_file_vec);
*/

}
