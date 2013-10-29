#ifndef DATA_STRUCTURE__AC_TIRE_HPP
#define DATA_STRUCTURE__AC_TIRE_HPP

/*                       Titles                                          Authors                        Date
 *- array create work items & work group                                 Chatsiri.rat								  08/02/2013
 */
#include <boost/unordered_map.hpp>
#include <vector>
#include <deque>
#include <map>
#include <set>

static size_t const AC_FAIL_STATE = -1;

namespace data_structure
{
    //------------------------- goto_function --------------------------------
    template<typename SymbolT, typename KeywordIterT>
    class goto_function
    {
			 public:
    //    private:
            typedef std::size_t state_t;
            typedef std::map<state_t, std::set<size_t> > output_function;
            typedef boost::unordered_map<SymbolT, state_t>  edges_t;
            typedef std::vector<edges_t> nodes_t;
            nodes_t graph_;
        public:

            goto_function(KeywordIterT kw_iter,
                    KeywordIterT  const&   kw_end,
                    output_function& output_f);

            std::size_t operator()(std::size_t state, SymbolT const& symbol)const;
            std::vector<std::map<std::size_t, std::set<std::size_t> > > const& get_nodes()const;
            void    enter(KeywordIterT const& keyword, state_t& newstate);

    };

    //----------------------- failure function --------------------------------
    template<typename SymbolT, typename KeywordIterT>
    class failure_function
    {
        private:
            typedef size_t state_t;
            typedef std::map<state_t, std::set<size_t> > output_function;
            std::vector<state_t> table_;

        public:
            failure_function(goto_function<SymbolT, KeywordIterT> const& _goto, output_function& output);
            state_t operator()(state_t state)const;
            inline void queue_edges(typename goto_function<SymbolT, KeywordIterT>::edges_t const& node,
                    std::deque<state_t>& queue);

    };

    template<typename SymbolT,
             typename KeywordIterT,
             typename InputIterT,
             typename CallbackT>
    class ac_tire
    {
        private:
            typedef size_t state_t;
            state_t state_;
            typedef std::map<state_t, std::set<size_t> > output_function;
            output_function output_;
            goto_function<SymbolT, KeywordIterT> goto_;
            failure_function<SymbolT, KeywordIterT> fail_;
            size_t where_;
        public:
						ac_tire(KeywordIterT const & kw_begin, KeywordIterT const & kw_end);
            void search(InputIterT input_it, InputIterT input_end, CallbackT& callback);
            SymbolT const& input(InputIterT *input_it);
            //void input(InputIterT * input_it);

    };


}

#endif
