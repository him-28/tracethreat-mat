#include "tbbscan/data_structure/actire_concurrency.hpp"
#include "tbb/cache_aligned_allocator.h"

#include <boost/assign/std/vector.hpp>
#include <boost/unordered_map.hpp>

#include "utils/file_offset_handler.hpp"

#include "scan/file_scan_policy.hpp"

#include "data_structure/actire_parallel.hpp"
#include "data_structure/ac_tire.hpp"

#include "filetypes/pe_file_controller.hpp"

#include "utils/base/system_code.hpp"

#include "utils/base/common.hpp"

#define FILE_ON 3

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
		char * binary_hex="a500e6b7a4b523235323234ae12b1e8bb";
 //"a50009cd21b44ce1aea";
    binary_hex_input.insert(binary_hex_input.end(), binary_hex, binary_hex + strlen(binary_hex));

    std::vector<std::string> keywords;
    std::string sig1 = "09cd21b80";
    std::string sig2 = "09cd21b44ce1aea";

    //Node of tree.
    std::vector<struct utils::meta_sig *>  nstr_vec;
    //Output function.
    tbb::concurrent_unordered_map<std::size_t,
        std::set<struct utils::meta_sig *> > output_fn;


    //boost::unordered_map<std::size_t,
    //    std::set<struct utils::meta_sig *> > output_fn;


    //struct utils::meta_sig n_str[0];
    //n_str->sig  = "09cd21b8014ccd215468";
    //n_str->virname   = "test_trojan";
    //n_str->sig_size  = sig.size();
    struct utils::meta_sig *n_str;

    nstr_vec.push_back(new struct utils::meta_sig);
    n_str = nstr_vec[0];
    n_str->sig  = "09cd21b80"; //sig_node to sig
    n_str->virname   = "test_trojan_a";
    n_str->sig_size  = sig1.size();
		sig_key_vec.push_back(std::string(n_str->sig));

    nstr_vec.push_back(new struct utils::meta_sig);
    n_str = nstr_vec[1];
    n_str->sig  = "09cd21b44ce1aea";
    n_str->virname   = "test_trojan_b";
    n_str->sig_size  = sig2.size();
		sig_key_vec.push_back(std::string(n_str->sig));

    nstr_vec.push_back(new struct utils::meta_sig);
    n_str = nstr_vec[2];
    n_str->sig  = "235323234ae12b1e8";
    n_str->virname   = "test_trojan_c";
    n_str->sig_size  = sig2.size();
		sig_key_vec.push_back(std::string(n_str->sig));

		nstr_vec.push_back(new struct utils::meta_sig);
    n_str = nstr_vec[3];
    n_str->sig  = "2353ab1e119d32c67a23ab3d";
    n_str->virname   = "test_trojan_d";
    n_str->sig_size  = sig2.size();
		sig_key_vec.push_back(std::string(n_str->sig));


    goto_function<char, tbbscan::tbb_allocator> goto_fn;
    goto_fn.create_goto(nstr_vec, output_fn);

    failure_function<char, tbbscan::tbb_allocator> failure_fn(goto_fn);
    failure_fn.create_failure(goto_fn, output_fn);


    actire_pe_concurrency<char, tbbscan::tbb_allocator> pe_parallel_search;
		results_callback<std::vector<std::string> > result(sig_key_vec);
    pe_parallel_search.search_basic(&goto_fn, &failure_fn, &output_fn, result, &binary_hex_input);
    //pe_parallel_search.search(&goto_fn, &failure_fn, &output_fn, result, &binary_hex_input);

		//std::string sigtype_code = tbbscan::actire_engine_factory<char, tbbscan::tbb_allocator>::
		//filetype_code_map(utils::pe_file);

		tbbscan::actire_engine_factory<char, tbbscan::tbb_allocator>::register_actire_type("pe_actire_engine",
			actire_pe_concurrency<char, tbbscan::tbb_allocator>::create);



}

