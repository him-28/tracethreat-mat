#include "data_structure/ac_tire.hpp"

namespace data_structure
{

typedef std::vector<std::set<std::size_t> > (*test_function_t)
(std::vector<std::string> const &, char const *, char const *);

std::vector<std::set<std::size_t> >
ac_test_function(std::vector<std::string> const & keywords, char const *begin, char const *end)
{
    ac_graph<char> ac(keywords.begin(), keywords.end());
    results <std::vector<std::string> > results_(keywords);
	
    ac.search(begin, end, results_);
    return results_.hits_;
}

//add matt_test_function
static std::vector<test_function_t> test_functions =
        boost::assign::list_of(&ac_test_function);

typedef struct {
    std::vector<std::string> keywords;
    char const *input;
    std::vector<std::set<std::size_t> > expected;
} TestData;


}
