#include "threadsyncocl/thread_sync.hpp"
#include "data_structure/ac_tire.hpp"
#include <boost/assign/list_of.hpp>
#include <vector>


using namespace controller;
using namespace data_structure;

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

}
