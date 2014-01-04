#ifndef DATA_STRUCTURE__AC_TIRE_HPP
#define DATA_STRUCTURE__AC_TIRE_HPP

/*                       Titles                                          Authors                        Date
 *-graph ac                                                             Chatsiri.rat			08/02/2013
 */
#include <cassert>
#include <deque>
#include <map>
#include <set>
#include <vector>
#include <typeinfo>
#include <cstdio>

#include <boost/unordered_map.hpp>
#include <boost/assign/list_of.hpp>


static size_t const AC_FAIL_STATE = -1;

namespace data_structure
{

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


template <typename SymbolT>
class ac_graph
{
// main public
public:
    typedef size_t state_t;

    // KeywordIterT is a sequence of sequences of SymbolT
    template <typename KeywordIterT>
    ac_graph(
            KeywordIterT const & kw_begin,
            KeywordIterT const & kw_end)
    :   goto_(kw_begin, kw_end, output_),
        fail_(goto_, output_),
        where_(0),
        state_(0)
    {
	
    }

    // input is sequence of symbols
    // CallbackT must be callable with (size_t what, size_t where)
//    template <typename InputIterT, typename CallbackT>
    void search(
          /*  InputIterT  input_it,
            InputIterT  input_end,*/
						char const *  input_it,
						char const * input_end,
            results<std::vector<std::string> >  & callback)
    {
				/*
				if(typeid(input_it) != typeid(char const *))
				{
						std::cout<<" Type Completed " <<std::endl;
				}
				*/
        for ( ; input_it != input_end; ++input_it, where_++)
        {
	
            char const & input = *input_it;
            {
                state_t next;
                while ((next = goto_(state_, input)) == AC_FAIL_STATE)
                    state_ = fail_(state_);
                state_ = next;
            }

            {
                std::set<std::size_t> const &out_node = output_[state_];
                typename std::set<size_t>::const_iterator output_it;
                for (output_it = out_node.begin(); output_it != out_node.end(); ++output_it)
                {
                    callback(*output_it, where_);
                }
            }
        }
    }

    // use this to start a new scan
    void reset() { state_ = 0; where_ = 0; }
// main private
private:
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
                KeywordIterT const & kw_end,
                output_function & output_f)
        {
						/*
						if(typeid(kw_iter) == typeid(char const&))
						{

						}
						*/

            state_t newstate = 0;
            size_t kw_index = 0;
            for ( ; kw_iter != kw_end; ++kw_iter)
            {
                enter(*kw_iter, newstate);
                output_f[newstate].insert(kw_index++);
            }
        }

        state_t operator()(state_t state, SymbolT const & symbol) const
        {
            //assert(state < graph_.size());
            edges_t const &node(graph_[state]);
            typename edges_t::const_iterator const &edge_it(node.find(symbol));
            if (edge_it != node.end())
            {
                return edge_it->second;
            }
            else
            {
                return (state == 0) ? 0 : AC_FAIL_STATE;
            }
        }

        nodes_t const & get_nodes() const { return graph_; }

    private:
        template <typename KeywordT>
        void enter(
                KeywordT const & keyword,
                state_t & newstate)
        {
            state_t state = 0;
            size_t index = 0;
            edges_t *node;
						if(typeid(keyword) != typeid(std::string)){
								std::cout<<" Is keyword " <<std::endl;
						}
            // follow existing symbol edges
            for ( ; index < keyword.size(); index++)
            {
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
            for ( ; index < keyword.size(); index++)
            {
                (*node)[keyword[index]] = ++newstate;
                state = newstate;
                node = &graph_[state];
            }
            assert(graph_.size() == state + 1);
        }

        nodes_t graph_;
    };
		//-------------------------------------End of Class go to function---------------------------------------

		//---------------------------   failure_function -----------------------------
    class failure_function
    {
    public:
        failure_function(
                goto_function const & _goto,
                output_function & output)
        :   table_(_goto.get_nodes().size(), AC_FAIL_STATE)
        {
            std::deque<state_t> queue;

            queue_edges(_goto.get_nodes()[0], queue);

            while (!queue.empty())
            {
                state_t r = queue.front();
                queue.pop_front();
                typename goto_function::edges_t const &node(_goto.get_nodes()[r]);
                typename goto_function::edges_t::const_iterator edge_it;
                for (edge_it = node.begin(); edge_it != node.end(); ++edge_it)
                {
                    std::pair<SymbolT, state_t> const &edge(*edge_it);
                    SymbolT const &a(edge.first);
                    state_t const &s(edge.second);

                    queue.push_back(s);
                    state_t state = table_[r];
                    while (_goto(state, a) == AC_FAIL_STATE)
                        state = table_[state];
                    table_[s] = _goto(state, a);
                    output[s].insert(
                            output[table_[s]].begin(),
                            output[table_[s]].end());
                }
            }
        }

        state_t operator()(state_t state) const
        {
            return table_[state];
        }

    private:
        std::vector<state_t> table_;

        /* queue nonfail edges */
        inline void queue_edges(
                typename goto_function::edges_t const & node,
                std::deque<state_t> & queue)
        {
            typename goto_function::edges_t::const_iterator edge_it;
            for (edge_it = node.begin(); edge_it != node.end(); ++edge_it)
            {
                std::pair<SymbolT, state_t> const &edge(*edge_it);
                queue.push_back(edge.second);
                table_[edge.second] = 0;
            }
        }
    };
		//--------------------------- End of  failure_function -----------------------------

    output_function output_;
    goto_function const goto_;
    failure_function const fail_;
    size_t where_;
    state_t state_;
};

}

#endif
