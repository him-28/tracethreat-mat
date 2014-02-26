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
                    KeywordIterT const& kw_begin,
                    KeywordIterT const& kw_end)
                :   goto_(kw_begin, kw_end, output_),
                    fail_(goto_, output_),
                    where_(0),
                    state_(0) {

            }

            // input is sequence of symbols
            // CallbackT must be callable with (size_t what, size_t where)
            //    template <typename InputIterT, typename CallbackT>
            void search(
                    /*  InputIterT  input_it,
                      InputIterT  input_end,*/
                    char const   *input_it,
                    char const *input_end,
                    results<std::vector<std::string> >&   callback) {
                /*
                if(typeid(input_it) != typeid(char const *))
                {
                		std::cout<<" Type Completed " <<std::endl;
                }
                */
                // Check size of graph_

                for ( ; input_it != input_end; ++input_it, where_++) {

                    char const& input = *input_it;
                    {
                        state_t next;
												//printf("--- Check Search ---\n");
                        //printf("search, input : %c \n", input);

                        while ((next = goto_(state_, input)) == AC_FAIL_STATE) {
														//printf("search, --- Check Fail ---\n");
														//printf("search, state send to file_ : %lu \n", state_);
                            state_ = fail_(state_);
                            //printf("search, fail_ state_ : %lu  \n", state_);
                        }

                        state_ = next;
                        //printf("search, next state_ : %lu \n", next);
                    }

                    {
                        //printf("-- Callback --\n");
                        //printf("-- callback, state_ : %lu \n", state_);
                        std::set<std::size_t> const& out_node = output_[state_];
                        typename std::set<size_t>::const_iterator output_it;

                        for (output_it = out_node.begin(); output_it != out_node.end(); ++output_it) {
                            //printf("-- callback, position where_ : %lu \n", where_);
                            callback(*output_it, where_);
                        }
                    }
                }
            }

            //get vector contain string, go_to and failture state to actire_parallel

            //output function
            std::map<state_t, std::set<size_t> >& get_output_function() {
                return output_;
            }

            //get graph
            std::vector<boost::unordered_map<SymbolT, state_t> > const& get_graph() {
                return goto_.get_nodes();
            }

            //table
            std::vector<state_t>& get_table() {
                return fail_.get_table();
            }

            // use this to start a new scan
            void reset() {
                state_ = 0;
                where_ = 0;
            }
            // main private
        private:
            typedef std::map<state_t, std::set<size_t> > output_function;
            //---------------------------Class go to function---------------------------------------
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
                        //printf("--- Check Goto_Function ---\n");
                        //printf("goto_function, Graph_ in goto_function size : %lu \n", graph_.size());
                        //printf("goto_function, operator(), State : %lu, Symbol : %c \n", state, symbol);
                        edges_t const& node(graph_[state]); // State for next state map
                        //printf("goto_function, Node from graph_ size : %lu \n", node.size());

                        typename edges_t::const_iterator const& edge_it(node.find(symbol));

                        if (edge_it != node.end()) {
                           // printf("goto_function, edge_it->second : %lu \n", edge_it->second);
                            return edge_it->second;
                        } else {
                           // printf("goto_function, State : %lu, or AC_FAIL_STATE \n", state);
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
												//printf("--- Check Enter ---\n");
                        // follow existing symbol edges
                        for ( ; index < keyword.size(); index++) {
                            // this node won't be initialized

                            //printf("enter, graph_.size : %lu , state : %lu \n", graph_.size(), state);
                            if (state == graph_.size()){
                                graph_.resize(state + 1);
														}

                            node = &graph_[state];

                            typename edges_t::iterator edge = node->find(keyword[index]);
                            //printf("enter, keyword :%c, state : %lu \n", keyword[index], state);

                            if (edge == node->end()) {
														//printf("enter, break if(edge == node->end())\n");
                                break;
                            }

                            state = edge->second;


                        }

                        // increase graph size by the number of remaining symbols
                        graph_.resize(graph_.size() + keyword.size() - index);
                        node = &graph_[state];
												//printf("--- Check insert data to node(ordered_map) ---\n");
                        //printf("enter berore for , state : %lu \n", state);

                        // generate new symbol edges
                        for ( ; index < keyword.size(); index++) {
                            (*node)[keyword[index]] = ++newstate;
                            //printf("enter, Node : %c , NewState : %lu \n", keyword[index], newstate);
                            state = newstate;
                            node = &graph_[state]; // Get New element for contain new map state.
                        }

                        assert(graph_.size() == state + 1);
                    }

                    nodes_t graph_;
            };
            //----------End of Class go to function---------------------------------------

            //---------------------------   failure_function -----------------------------
            class failure_function
            {
                public:
                    failure_function(
                            goto_function const& _goto,
                            output_function& output)
                        :   table_(_goto.get_nodes().size(), AC_FAIL_STATE) {
                        std::deque<state_t> queue;

                        queue_edges(_goto.get_nodes()[0], queue);

                        while (!queue.empty()) {
                            state_t r = queue.front();
                            queue.pop_front();
                            typename goto_function::edges_t const& node(_goto.get_nodes()[r]);
                            typename goto_function::edges_t::const_iterator edge_it;

                            for (edge_it = node.begin(); edge_it != node.end(); ++edge_it) {
                                std::pair<SymbolT, state_t> const& edge(*edge_it);
                                SymbolT const& a(edge.first);
                                state_t const& s(edge.second);

                                //printf("failure_function, Symbol a: %c, State_t s : %lu, Table r: %lu \n", a, s, r);
                                queue.push_back(s);
                                state_t state = table_[r];

                                //printf("failure_function, State from table : %lu \n", state);

                                while (_goto(state, a) == AC_FAIL_STATE) {
                                    state = table_[state];

                                    //printf("failure_function, while fail state on table : %lu  \n", state);

                                }

                                //printf("failure_function, a sent to goto again : %c \n", a);

                                table_[s] = _goto(state, a);

                                //printf("failure_function, table_[s] : %lu, s: %lu \n", table_[s], s);

                                output[s].insert(
                                        output[table_[s]].begin(),
                                        output[table_[s]].end());
                            }
                        }
                    }

                    state_t operator()(state_t state) const {
                        //printf("failure_function, return operator() : %lu \n", table_[state]);
                        return table_[state];
                    }

                    std::vector<state_t>& get_table() {
                        return table_;
                    }

                private:
                    std::vector<state_t> table_;

                    /* queue nonfail edges */
                    inline void queue_edges(
                            typename goto_function::edges_t const& node,
                            std::deque<state_t>& queue) {
                        typename goto_function::edges_t::const_iterator edge_it;

                        for (edge_it = node.begin(); edge_it != node.end(); ++edge_it) {
                            std::pair<SymbolT, state_t> const& edge(*edge_it);
                            //printf("queue_edges, edge_id value : %lu \n", edge.second);
                            queue.push_back(edge.second); // push_back opsition of keywords [1 and 7]
                            table_[edge.second] = 0; // insert position of keywords [edge.second]
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
