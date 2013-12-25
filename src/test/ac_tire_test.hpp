#include "data_structure/ac_tire.hpp"
#include <boost/assign/list_of.hpp>
#include <vector>

using namespace data_structure;

class InitDataAc : public ::testing::Test
{

    protected:

        typedef struct {
            std::vector<std::string> keywords;
            char const *input;
        } test_data;

        template<typename KeywordStoreT>
        struct results {
            results(KeywordStoreT const& keywords, bool summary = false)
                : keywords_(keywords),
                  hits_(keywords.size()),
                  summary_(summary) {

            }

            void operator()(std::size_t what, std::size_t where) {

                if(!summary_)
                    std::cerr << "[" << where << "]" << keywords_[what] << std::endl;
                else
                    std::cerr << ".";

                hits_[what].insert(where);
            }
            KeywordStoreT const& keywords_;
            std::vector<std::set<std::size_t> > hits_;
            bool summary_;

        };

        /*
                std::vector<std::set<std::size_t> > ac_test_function(std::vector<std::string> const& keywords,
                        char *begin,
                        char  *end) {
                    ac_tire<char,
                            std::vector<std::string>::const_iterator,
                            char,
                            results<std::vector<std::string> > > ac(keywords.begin(), keywords.end());
                    results<std::vector<std::string> > result(keywords);
                    ac.search(*begin, *end, result);
                    return result.hits_;

                }
        */

        virtual void SetUp() {
            td[0].keywords = boost::assign::list_of("he")("his")("her");
            td[0].input    = "hi";
            /*
            for(int i = 0; i  < 1; i++) {
            		char * input_tmp = const_cast<char*>(&td[i].input[0]);
            		char * end_input_tmp = const_cast<char*>(&td[i].input[strlen(td[i].input)]);
            ac_test_function(td[i].keywords,
            input_tmp,
            end_input_tmp);
            } */
        }
        test_data td[1];

};


TEST_F(InitDataAc , ac_add_list_str)
{
    for(int i = 0; i  < 1; i++) {
        char input_tmp = td[i].input[0];
        char end_input_tmp = td[i].input[strlen(td[i].input)];
        //				char k_begin = td[i].keywords.begin();
        //				char k_end   = td[i].keywords.end();
        /*
        ac_tire<char,
                std::vector<std::string>::const_iterator,
                char,
                std::string,
                results<std::vector<std::string> >
                >
                ac(td[i].keywords.begin(), td[i].keywords.end());
				*/
        //				ac(std::string("data"),std::string("value"));
        // results<std::vector<std::string> > result(td[i].keywords);
        // ac.search(input_tmp, end_input_tmp, result);
    }

}
