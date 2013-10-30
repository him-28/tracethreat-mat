#include "data_structure/ac_tire.hpp"

namespace data_structure
{
    //----------------------------  ac_tire ----------------------------------
    template<typename SymbolT,
             typename KeywordIterT,
             typename InputIterT,
             typename KeywordT,
             typename CallbackT>
    ac_tire<SymbolT,
            KeywordIterT,
            InputIterT,
            KeywordT,
            CallbackT>::ac_tire(KeywordIterT const& kw_begin, KeywordIterT const& kw_end)
                : goto_(kw_begin, kw_end, output_),
                  fail_(goto_, output_),
                  where_(0),
                  state_(0)
    {

    }


    template<typename SymbolT,
             typename KeywordIterT,
             typename InputIterT,
             typename KeywordT,
             typename CallbackT>
    void ac_tire<SymbolT,
         KeywordIterT,
         InputIterT,
         KeywordT,
         CallbackT>::search(InputIterT input_it,
                 InputIterT input_end,
                 CallbackT& callback)
    {
        for(; input_it != input_end; ++input_it, where_++) {
            input(input_it);
            std::set<size_t> const& out_node = output_[state_];
            typename std::set<size_t>::const_iterator output_it;

            for(output_it = out_node.begin(); output_it != out_node.end(); ++output_it) {
                callback(*output_it, where_);
            }
        }
    }

    template<typename SymbolT,
    typename KeywordIterT,
    typename InputIterT,
    typename KeywordT,
    typename CallbackT>
    SymbolT const ac_tire<SymbolT,
            KeywordIterT,
            InputIterT,
            KeywordT,
            CallbackT>::input(InputIterT input_it)
    {
        state_t next;

        while((next = goto_(state_, input(input_it))) == AC_FAIL_STATE ) {
            //    state_ = fail_(state_);
        }

        state_ = next;
        return state_;
    }

    //------------------------- goto_function --------------------------------
    template<typename SymbolT, typename KeywordIterT, typename KeywordT>
    goto_function<SymbolT, KeywordIterT, KeywordT>::goto_function(KeywordIterT kw_iter,
            KeywordIterT  const&   kw_end,
            output_function& output_f)
    {
        state_t newstate = 0;
        size_t  kw_index = 0;

        for(; kw_iter != kw_end; ++kw_iter) {
            enter(*kw_iter, newstate);
            output_f[newstate].insert(kw_index++);
        }

    }


    template<typename SymbolT, typename KeywordIterT, typename KeywordT>
    void goto_function<SymbolT, KeywordIterT, KeywordT>::enter(KeywordT const& keyword,
            state_t& newstate)
    {
        state_t state = 0;
        size_t   index = 0;
        edges_t *node;

        for(; index < keyword.size(); index++) {
            if(state ==  graph_.size()) {
                graph_.resize(state + 1);
            }

            node = &graph_[state];
            typename edges_t::iterator edge = node->find(keyword[index]);

            if(edge == node->end())
                break;

            state = edge->second;
        }

        graph_.resize(graph_.size() + keyword.size() - index);
        node = &graph_[state];

        for(; index < keyword.size(); index++) {
            (*node)[keyword[index]] = ++newstate;
            state = newstate;
            node = &graph_[state];
        }
    }

    template<typename SymbolT, typename KeywordIterT, typename KeywordT>
    std::size_t goto_function<SymbolT, KeywordIterT, KeywordT>::operator()(state_t state, SymbolT const& symbol)const
    {
        edges_t const& node(graph_[state]);
        typename edges_t::const_iterator const& edge_it(node.find(symbol));

        if(edge_it != node.end()) {
            return edge_it->second;
        } else {
            return (state == 0)? 0 : AC_FAIL_STATE;
        }
    }


    template<typename SymbolT, typename KeywordIterT, typename KeywordT>
    std::vector<boost::unordered_map<SymbolT, std::size_t> >
    const& goto_function<SymbolT, KeywordIterT, KeywordT>::get_nodes()const
    {
        return graph_;
    }

    //------------------------------- failure_function----------------------------------
    template<typename SymbolT, typename KeywordIterT, typename KeywordT>
    failure_function<SymbolT, KeywordIterT, KeywordT>::failure_function(
            goto_function<SymbolT, KeywordIterT, KeywordT> const& _goto,
            output_function& output)
    {
        std::deque<state_t> queue;
        queue_edges(_goto.get_nodes()[0], queue);

        while(!queue.empty()) {
            state_t r = queue.front();
            queue.pop_front();

            typename goto_function<SymbolT, KeywordIterT, KeywordT>
            ::edges_t const& node(_goto.get_nodes()[r]);
            typename goto_function<SymbolT, KeywordIterT, KeywordT>
            ::edges_t::const_iterator edge_it;

            for(edge_it = node.begin(); edge_it != node.end(); ++edge_it) {
                std::pair<SymbolT, state_t> const& edge(*edge_it);
                SymbolT const& a(edge.first);
                state_t const& s(edge.second);

                queue.push_back(s);
                state_t state = table_[r];

                while(_goto(state, a) == AC_FAIL_STATE) {
                    state = table_[state];
                }

                table_[s] = _goto(state, a);
                output[s].insert( output[table_[s]].begin(), output[table_[s]].end() );
            }

        }

    }

    template<typename SymbolT, typename KeywordIterT, typename KeywordT>
    std::size_t failure_function<SymbolT, KeywordIterT, KeywordT>::operator()(state_t state)const
    {
        return table_[state];
    }

    template<typename SymbolT, typename KeywordIterT, typename KeywordT>
    inline void failure_function<SymbolT, KeywordIterT, KeywordT>::queue_edges(
            typename goto_function<SymbolT, KeywordIterT, KeywordT>::edges_t const& node,
            std::deque<std::size_t>& queue)
    {
        typename goto_function<SymbolT, KeywordIterT, KeywordT>::edges_t::const_iterator edge_it;

        for(edge_it = node.begin(); edge_it != node.end(); ++edge_it) {
            std::pair<SymbolT, std::size_t> const& edge(*edge_it);
            queue.push_back(edge.second);
            table_[edge.second] = 0;
        }

    }

}
