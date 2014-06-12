#include "tbbscan/data_structure/actire_concurrency.hpp"
#include "tbb/cache_aligned_allocator.h"
#include <boost/assign/std/vector.hpp>
#include <boost/unordered_map.hpp>
#include "utils/file_offset_handler.hpp"
#include "scan/file_scan_policy.hpp"
#include "data_structure/actire_parallel.hpp"
#include "data_structure/ac_tire.hpp"
#include "filetypes/pe_file_controller.hpp"

#define FILE_ON 2

namespace data_str = data_structure;
namespace fpolicy  = policy;
using namespace utils;//   = util;
using tbbscan::goto_function;
using tbbscan::failure_function;
using tbbscan::actire_pe_concurrency;
using tbbscan::results_callback;

TEST(ACTireConcurrency, goto_function)
{
		std::vector<std::string> sig_key_vec;
    //binary input from file (Test only)
    std::vector<char> binary_hex_input;
		char * binary_hex= "a4b523235323234ae12b1e8bb";
    binary_hex_input.insert(binary_hex_input.end(), binary_hex, binary_hex + strlen(binary_hex));

    std::vector<std::string> keywords;
    std::string sig_node1 = "09cd21b80";
    std::string sig_node2 = "09cd21b44ce1aea";

    //Node of tree.
    std::vector<struct tbbscan::node_str *>  nstr_vec;
    //Output function.
    //tbb::concurrent_unordered_map<std::size_t,
    //    std::set<struct tbbscan::node_str *> > output_fn;


    boost::unordered_map<std::size_t,
        std::set<struct tbbscan::node_str *> > output_fn;


    //struct tbbscan::node_str n_str[0];
    //n_str->sig_node  = "09cd21b8014ccd215468";
    //n_str->virname   = "test_trojan";
    //n_str->sig_size  = sig_node.size();
    struct tbbscan::node_str *n_str;

    nstr_vec.push_back(new struct tbbscan::node_str);
    n_str = nstr_vec[0];
    n_str->sig_node  = "09cd21b80";
    n_str->virname   = "test_trojan_a";
    n_str->sig_size  = sig_node1.size();
		sig_key_vec.push_back(std::string(n_str->sig_node));

    nstr_vec.push_back(new struct tbbscan::node_str);
    n_str = nstr_vec[1];
    n_str->sig_node  = "09cd21b44ce1aea";
    n_str->virname   = "test_trojan_b";
    n_str->sig_size  = sig_node2.size();
		sig_key_vec.push_back(std::string(n_str->sig_node));

    nstr_vec.push_back(new struct tbbscan::node_str);
    n_str = nstr_vec[2];
    n_str->sig_node  = "235323234ae12b1e8";
    n_str->virname   = "test_trojan_c";
    n_str->sig_size  = sig_node2.size();
		sig_key_vec.push_back(std::string(n_str->sig_node));


    goto_function<char, tbbscan::tbb_allocator> goto_fn;
    goto_fn.create_goto(nstr_vec, output_fn);

    failure_function<char, tbbscan::tbb_allocator> failure_fn(goto_fn);
    failure_fn.create_failure(goto_fn, output_fn);


    actire_pe_concurrency<char, tbbscan::tbb_allocator> pe_parallel_search;
		results_callback<std::vector<std::string> > result(sig_key_vec);
		std::cout<<"Map str Ouput size " << output_fn.size() <<std::endl;
    pe_parallel_search.search_basic(&goto_fn, &failure_fn, &output_fn, result, &binary_hex_input);

}

