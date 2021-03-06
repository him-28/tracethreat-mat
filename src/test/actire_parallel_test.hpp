#include "data_structure/ac_tire.hpp"
#include "data_structure/actire_parallel.hpp"

#include "msg/message_tracethreat.pb.h"

/**
* @brief Create Testing insert data to opencl string.
*/
using namespace data_structure;


class InitDataAcTireParallel : public ::testing::Test
{
		public:

    typedef scan_threat::InfectedFileInfo  threatinfo_type;

    typedef	std::vector<threatinfo_type *>  rcb_container_type;

    protected:

        typedef struct {
            std::vector<std::string> keywords;
            char const *input;
        } test_data;

        std::vector<std::set<std::size_t> > ac_test_function(std::vector<std::string> const& keywords,
                char const *begin,
                char const *end) {

            ac_graph<char> ac(keywords.begin(), keywords.end());

						//test with actire_parallel
						typedef boost::unordered_map<char, size_t>  unordered_map;
						actire_parallel<char, size_t, unordered_map, std::vector<unordered_map> >  acp;
						acp.prepare_graph(&ac.get_graph());

            results<std::vector<std::string> > result(keywords, false);
					
            ac.search(begin, end, result);
            return result.hits_;

        }

        virtual void SetUp() {
            //td[0].keywords = boost::assign::list_of("4bc647")("4b537db2e3a337")("4b53abf");
            //td[0].input    = "a824bc647c46d04b537db2e3a33711";
            //td[0].keywords = boost::assign::list_of("4bc647")("537db2e3a337");
            td[0].keywords = {"537de", "4bc647", "4bc647ab53", "ab53", "537des" };
            td[0].input    = "a82a3f7daca1e4bc647c46d0dd553e637b06cc23547783ff91813d91fa3a197a63254331c0ac3c2189d138824797b800fdd73bdc8858081bb1e8e386a6033bc684454207b6997537db2e3a33711cd223db32ee49905a39a687bec057daa582a6a2b532e268b211a7529f4459b7102c2549e42d36344f53aece6b258f5904a4c0dec27dfbe8c61e9ee7885a57913cbf508322184e4b65";

        }
        test_data td[1];

};

TEST_F(InitDataAcTireParallel, actire_parallel_init)
{
            for(int i = 0; i  < 1; i++) {
                char *input_tmp = const_cast<char *>(&td[i].input[0]);
                char *end_input_tmp = const_cast<char *>(&td[i].input[strlen(td[i].input)]);
                ac_test_function(td[i].keywords,
                        input_tmp,
                        end_input_tmp);
            }

}

