#include "data_structure/ac_tire.hpp"

namespace data_structure
{
//using namespace std;
//using namespace boost::assign;
/*
template <typename KeywordStoreT>
struct results
{
    results(KeywordStoreT const & keywords, bool summary = false)
    :   keywords_(keywords),
        hits_(keywords.size()),
        summary_(summary)
    {
    }

    void operator()(size_t what, size_t where)
    {
        if (!summary_)
            std::cerr << "[" << where << "]" << keywords_[what] << std::endl;
        else
            std::cerr << ".";
        hits_[what].insert(where);
    }

    KeywordStoreT const &keywords_;
    std::vector<std::set<std::size_t> > hits_;
    bool summary_;
};
*/

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
