/*
* Copyright 2014 MTSec, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*  Titles			                                                     Authors	          Date
 *  -Init ACTire test with data.                                  R.Chatsiri
 */


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

        std::vector<std::set<std::size_t> > ac_test_function(std::vector<std::string> const& keywords,
                char const *begin,
                char const *end) {

            ac_graph<char> ac(keywords.begin(), keywords.end());

            results<std::vector<std::string> > result(keywords);
            ac.search(begin, end, result);
            return result.hits_;

        }

        virtual void SetUp() {
            //td[0].keywords = boost::assign::list_of("4bc647")("537db2e3a337");
						td[0].keywords = {"09cd21b80", "09cd21b44ce1aea" };
//boost::assign::list_of("09cd21b80")("09cd21b44ce1aea");
            td[0].input    = "a82a3f7daca1e4bc647c46d0dd553e637b06cc23547783ff91813";
/*
            td[1].keywords = boost::assign::list_of("4bc647")("537db2e3a337");
            td[1].input    = "a82a3f7daca1e4bc647c46d0dd553e637b06cc23547783ff91813d91fa3a197a63254331c0ac3c2189d138824797b800fdd73bdc8858081bb1e8e386a6033bc684454207b6997537db2e3a33711cd223db32ee49905a39a687bec057daa582a6a2b532e268b211a7529f4459b7102c2549e42d36344f53aece6b258f5904a4c0dec27dfbe8c61e9ee7885a57913cbf508322184e4b65";
*/
        }
        test_data td[1];

};


TEST_F(InitDataAc , ac_add_list_str)
{

    for(int i = 0; i  < 1; i++) {
        char *input_tmp = const_cast<char *>(&td[i].input[0]);
        char *end_input_tmp = const_cast<char *>(&td[i].input[strlen(td[i].input)]);
        ac_test_function(td[i].keywords,
                input_tmp,
                end_input_tmp);
    }

}
