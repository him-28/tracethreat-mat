#include "threadsyncocl/thread_sync.hpp"
#include "data_structure/ac_tire.hpp"
#include <boost/assign/list_of.hpp>
#include <vector>

// file_scan_policy
#include "scan/file_scan_policy.hpp"
//#include "scan/pe_file_policy.hpp"
//#include "filetypes/pe.hpp"
//#include "scan/pe_file_policy.hpp"

#include "utils/file_offset_handler.hpp"

#define FILE_NO 1


using namespace controller;
using namespace data_structure;
using namespace policy;
using namespace util;

//struct results<std::vector<std::string> >;
/*
typedef struct {
    std::vector<std::string> keywords;
    char const *input;
} test_data;
*/

/*
typedef std::vector<std::set<std::size_t> > (*test_function_t)(std::vector<std::string> const&, char const *, char const *);
*/

/*
std::vector<std::set<std::size_t> > ac_test_function(std::vector<std::string> const& keywords,
        char const *begin,
        char const  *end)
{
	//	/*
    ac_graph<char,
            std::vector<std::string>::const_iterator,
            std::vector<std::string>::const_iterator,
            std::string,
            results<std::vector<std::string> >
            >
            ac(keywords.begin(), keywords.end());
    results<std::vector<std::string> > result(keywords);
  //		*/
 //   ac.search(begin, end, result);
  //  return result.hits_;

//}

//static std::vector<test_function_t> test_functions = boost::assign::list_of(&ac_test_function);


/*
						test_data td[1];

            td[0].keywords = boost::assign::list_of("he")("his")("her");
            td[0].input    = "hi";
            for(int i = 0; i  < 1; i++) {
            		char * input_tmp = const_cast<char*>(&td[i].input[0]);
            		char * end_input_tmp = const_cast<char*>(&td[i].input[strlen(td[i].input)]);
            ac_test_function(td[i].keywords,
            input_tmp,
            end_input_tmp);
            }
*/


//test file_scan_policy
void test_file_scan_policy(){


				std::list<std::string> list_file_type;
        struct MAPPED_FILE_PE *s_mapped_fpe[FILE_NO];
        std::vector<MAPPED_FILE_PE *> mapped_file_vec;

            char const *file_name_offset[FILE_NO];

            file_name_offset[0] = "/home/chatsiri/sda1/workspacecpp/clamav-devel/test/Crack.exe";

            for(int count_file = 0; count_file < 	FILE_NO; count_file++) {
                list_file_type.push_back(file_name_offset[count_file]);

                s_mapped_fpe[count_file]	 = (struct MAPPED_FILE_PE *)malloc(sizeof(s_mapped_fpe));

                mapped_file_vec.push_back(s_mapped_fpe[count_file]);

            }

    // list_file_tye insert file name, s_mapped_fpe inserted  file_type details.
    file_offset_handler<struct common_filetype, struct MAPPED_FILE_PE>  fileoffset_h;

    std::vector<struct MAPPED_FILE_PE *> mapf_vec = fileoffset_h.get_mapped_file();
    typename std::vector<struct MAPPED_FILE_PE *>::iterator iter_mapf_vec;

    for(iter_mapf_vec = mapf_vec.begin(); iter_mapf_vec != mapf_vec.end(); ++iter_mapf_vec) {
				MAPPED_FILE_PE * mf_pe  = *iter_mapf_vec;
				size_t data_size = mf_pe->size;
			  std::cout<<"Data size  : " << data_size <<std::endl;	
				//send to pe get header
				
				
        // offset for scanning data

    }

	  pe_file_policy<struct MAPPED_FILE_PE>  pef_policy;// =  new pe_file_policy<struct MAPPED_FILE_PE>();	
   /*
   scan_file_policy<
				MAPPED_FILE_PE,
				pe_policy_is<pe_file_policy<MAPPED_FILE_PE>, MAPPED_FILE_PE> 
				> sf_policy;
    sf_policy.scan_pe(&pef_policy);
		*/
        
}

int main()
{

/*
    test_data td[1];

    td[0].keywords = boost::assign::list_of("he")("his")("her");
    td[0].input    = "hi";

    for(std::vector<test_function_t>::iterator it = test_functions.begin();
            it != test_functions.end();
            ++it) {
        for(int i =0; i < 1; i++) {
            (**it)(td[i].keywords, &td[i].input[0], &td[i].input[strlen(td[i].input)]);
            std::cout<<"pass " <<std::endl;
        }

    }

*/

/*   
        for(int i = 0; i  < 1; i++) {
            char input_tmp = td[i].input[0];
            char end_input_tmp = td[i].input[strlen(td[i].input)];
            ac_graph<char,
                    std::vector<std::string>::iterator,
                    char,
                    std::string,
                    results<std::vector<std::string> >
                    >
                    ac(td[i].keywords.begin(), td[i].keywords.end());
            //				ac(std::string("data"),std::string("value"));
            results<std::vector<std::string> > result(td[i].keywords);
            ac.search(input_tmp, end_input_tmp, result);

        }
  */  

//test file_scan_policy
test_file_scan_policy();

}
