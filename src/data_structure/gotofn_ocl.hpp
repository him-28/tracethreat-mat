#ifndef GOTOFN_OCL__HPP
#define GOTOFN_OCL__HPP

/*                       Titles                                          Authors                        Date
 *
 */

#include <iostream>

typedef std::map<state_t, std::set<size_t> > output_function;
//-------------------------------------Class go to function---------------------------------------
class goto_function
{
    public:
        typedef boost::unordered_map<SymbolT, state_t> edges_t;

        typedef std::vector<edges_t> nodes_t;

        template <typename KeywordIterT>
        goto_function(
                KeywordIterT kw_iter,
                KeywordIterT const& kw_end,
                output_function& output_f) {
            /*
            if(typeid(kw_iter) == typeid(char const&))
            {

            }
            */

            state_t newstate = 0;
            size_t kw_index = 0;

            for ( ; kw_iter != kw_end; ++kw_iter) {
                enter(*kw_iter, newstate);
                output_f[newstate].insert(kw_index++);
            }
        }

        state_t operator()(state_t state, SymbolT const& symbol) const {
            //assert(state < graph_.size());
            edges_t const& node(graph_[state]);
            typename edges_t::const_iterator const& edge_it(node.find(symbol));

            if (edge_it != node.end()) {
                return edge_it->second;
            } else {
                return (state == 0) ? 0 : AC_FAIL_STATE;
            }
        }

        nodes_t const& get_nodes() const {
            return graph_;
        }

    private:
        template <typename KeywordT>
        void enter(
                KeywordT const& keyword,
                state_t& newstate) {
            state_t state = 0;
            size_t index = 0;
            edges_t *node;

            if(typeid(keyword) != typeid(std::string)) {
                std::cout<<" Is keyword " <<std::endl;
            }

            // follow existing symbol edges
            for ( ; index < keyword.size(); index++) {
                // this node won't be initialized
                if (state == graph_.size())
                    graph_.resize(state + 1);

                node = &graph_[state];
                typename edges_t::iterator edge = node->find(keyword[index]);

                if (edge == node->end()) break;

                state = edge->second;
            }

            // increase graph size by the number of remaining symbols
            graph_.resize(graph_.size() + keyword.size() - index);
            node = &graph_[state];

            // generate new symbol edges
            for ( ; index < keyword.size(); index++) {
                (*node)[keyword[index]] = ++newstate;
                state = newstate;
                node = &graph_[state];
            }

            assert(graph_.size() == state + 1);
        }

        nodes_t graph_;
};


#endif /* GOTOFN_OCL__HPP */



