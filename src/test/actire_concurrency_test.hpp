#include "tbbscan/data_structure/actire_concurrency.hpp"
#include "tbb/cache_aligned_allocator.h"
#include <boost/assign/std/vector.hpp>

#include "utils/file_offset_handler.hpp"
#include "scan/file_scan_policy.hpp"
#include "data_structure/actire_parallel.hpp"
#include "data_structure/ac_tire.hpp"
#include "filetypes/pe_file_controller.hpp"

#define FILE_ON 1

namespace data_str = data_structure;
namespace fpolicy  = policy;
using namespace utils;//   = util;
using tbbscan::goto_function;
using tbbscan::failure_function;


TEST(ACTireConcurrency, goto_function)
{
      std::vector<std::string> keywords;
			std::string sig_node1 = "09cd21b80";
			std::string sig_node2 = "09cd21b44ce1aea";
	
			std::vector<struct tbbscan::node_str*>  nstr_vec;
			tbb::concurrent_unordered_map<std::size_t, 
					std::set<struct tbbscan::node_str*> > output_fn;
			//struct tbbscan::node_str n_str[0];
			//n_str->sig_node  = "09cd21b8014ccd215468";
			//n_str->virname   = "test_trojan";
			//n_str->sig_size  = sig_node.size();
				struct tbbscan::node_str * n_str; 

		    nstr_vec.push_back(new struct tbbscan::node_str);
			  n_str = nstr_vec[0];
			  n_str->sig_node  = "09cd21b80";
			  n_str->virname   = "test_trojan_a";
			  n_str->sig_size  = sig_node1.size();

		    nstr_vec.push_back(new struct tbbscan::node_str);
				n_str = nstr_vec[1];
			  n_str->sig_node  = "09cd21b44ce1aea";
			  n_str->virname   = "test_trojan_b";
			  n_str->sig_size  = sig_node2.size();

				goto_function<char, tbbscan::tbb_allocator> goto_fn;	
			  goto_fn.create_goto(nstr_vec, output_fn);
	
				failure_function<char, tbbscan::tbb_allocator> failure_fn(goto_fn);
				failure_fn.create_failure(goto_fn, output_fn);				


}

