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
    template<typename SymbolT, typename KeywordIterT, typename KeywordT>
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
            std::vector<boost::unordered_map<SymbolT, std::size_t> > const& get_nodes()const;
            void    enter(KeywordT const& keyword, state_t& newstate);

    };

    //----------------------- failure function --------------------------------
    template<typename SymbolT, typename KeywordIterT, typename KeywordT>
    class failure_function
    {
        private:
            typedef size_t state_t;
            typedef std::map<state_t, std::set<size_t> > output_function;
            std::vector<state_t> table_;

        public:
            failure_function(goto_function<SymbolT, KeywordIterT, KeywordT> const& _goto, output_function& output);
            state_t operator()(state_t state)const;
            inline void queue_edges(typename goto_function<SymbolT, KeywordIterT, KeywordT>::edges_t const& node,
                    std::deque<state_t>& queue);

    };

    //----------------------------------- results -----------------------------------------
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


    template<typename SymbolT,
             typename KeywordIterT,
             typename InputIterT,
             typename KeywordT,
             typename CallbackT
             >
    class ac_tire
    {
        private:
            typedef size_t state_t;
            state_t state_;
            typedef std::map<state_t, std::set<size_t> > output_function;
            output_function output_;
            goto_function<SymbolT, KeywordIterT, KeywordT> goto_;
             failure_function<SymbolT, KeywordIterT, KeywordT> fail_;
            size_t where_;
        public:
            ac_tire(KeywordIterT const& kw_begin, KeywordIterT const& kw_end);
            void search(InputIterT input_it, InputIterT input_end, CallbackT& callback);
            SymbolT const input(InputIterT input_it);
            //void input(InputIterT * input_it);

    };


    template class  ac_tire
    <char,
    std::vector<std::string>::iterator,
    std::vector<std::string>::iterator,
    std::string,
    results<std::vector<std::string> >
    >;

    template class goto_function <char, std::vector<std::string>::const_iterator, std::string>;
    template class failure_function <char,  std::vector<std::string>::const_iterator, std::string>;

}

#endif
