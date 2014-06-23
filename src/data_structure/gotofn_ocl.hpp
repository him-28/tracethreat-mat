#ifndef GOTOFN_OCL__HPP
#define GOTOFN_OCL__HPP

/*                       Titles                                          Authors                        Date
 *
 */

#include <iostream>
#include <map>
#include <vector>

namespace data_structure{

template<typename SymbolT, typename KeywordIterT, typename KeywordT>
class goto_function
{
    public:

        goto_function(
                KeywordIterT kw_iter,
                KeywordIterT const& kw_end,
                output_function& output_f);
 
        state_t operator()(state_t state, SymbolT const& symbol) const;

        nodes_t const& get_nodes() const;

        void enter(KeywordT const& keyword, state_t& newstate);

    private:
        typedef size_t state_t;

        typedef boost::unordered_map<SymbolT, state_t> edges_t;

        typedef std::vector<edges_t> nodes_t;
        nodes_t graph_;

        typedef std::map<state_t, std::set<size_t> > output_function;

};


}



#endif /* GOTOFN_OCL__HPP */



