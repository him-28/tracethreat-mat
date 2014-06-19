#include "tbbscan/data_structure/actire_concurrency.hpp"
#include "tbb/cache_aligned_allocator.h"
#include "tbb/concurrent_vector.h"

#include <boost/assign/std/vector.hpp>
#include <boost/unordered_map.hpp>

#include "utils/file_offset_handler.hpp"

#include "scan/file_scan_policy.hpp"

//#include "data_structure/actire_parallel.hpp"
//#include "data_structure/ac_tire.hpp"

//#include "filetypes/pe_file_controller.hpp"

#include "utils/base/system_code.hpp"

//#include "utils/base/common.hpp"

#include "utils/convert.hpp"

//#define FILE_ON 3

//namespace data_str = data_structure;
//namespace fpolicy  = policy;
using namespace utils;//   = util;
using tbbscan::goto_function;
using tbbscan::failure_function;

//using tbbscan::actire_pe_engine;
//using tbbscan::result_callback;


class ACTireConcurrency : public ::testing::Test
{

    protected:


        virtual void SetUp() {


            std::string sig1 = "09cd21b80";
            std::string sig2 = "09cd21b44ce1aea";

            file_name_binary_hex = "trojan_test.exe";
            char *binary_hex="a82a3f709cd21b44ce1aeadaca1e4bc647c46d0dd553e637b06cc23547783ff91813";

            //"a50009cd21b44ce1aea";
            //binary_hex_input.insert(binary_hex_input.end(), binary_hex, binary_hex + strlen(binary_hex));
						memcpy(&binary_hex_input[*binary_hex_input.grow_by(strlen(binary_hex))], 
								binary_hex, 
								strlen(binary_hex)+1);
            //boost::unordered_map<std::size_t,
            //    std::set<struct utils::meta_sig *> > output_fn;


            //struct utils::meta_sig n_str[0];
            //n_str->sig  = "09cd21b8014ccd215468";
            //n_str->virname   = "test_trojan";
            //n_str->sig_size  = sig.size();

            nstr_vec.push_back(new struct utils::meta_sig);
            n_str = nstr_vec[0];
            n_str->sig  = "09cd21b80"; //sig_node to sig
            n_str->virname   = "test_trojan_a";
            n_str->sig_type  = utils::pe_file;
            n_str->sig_size  = sig1.size();
            sig_key_vec.push_back(std::string(n_str->sig));

            nstr_vec.push_back(new struct utils::meta_sig);
            n_str = nstr_vec[1];
            n_str->sig  = "09cd21b44ce1aea";
            n_str->virname   = "test_trojan_b";
            n_str->sig_type  = utils::pe_file;
            n_str->sig_size  = sig2.size();
            sig_key_vec.push_back(std::string(n_str->sig));
						
            nstr_vec.push_back(new struct utils::meta_sig);
            n_str = nstr_vec[2];
            n_str->sig  = "235323234ae12b1e8";
            n_str->virname   = "test_trojan_c";
            n_str->sig_type  = utils::pe_file;
            n_str->sig_size  = sig2.size();
            sig_key_vec.push_back(std::string(n_str->sig));
							
            nstr_vec.push_back(new struct utils::meta_sig);
            n_str = nstr_vec[3];
            n_str->sig  = "2353ab1e119d32c67a23ab3d";
            n_str->virname   = "test_trojan_d";
            n_str->sig_type  = utils::pe_file;
            n_str->sig_size  = sig2.size();
            sig_key_vec.push_back(std::string(n_str->sig));
						
        }

        std::string file_name_binary_hex;

        std::vector<std::string> sig_key_vec;
        //binary input from file (Test only)
        tbb::concurrent_vector<char> binary_hex_input;

        std::vector<std::string> keywords;

        //Node of tree.
        std::vector<struct utils::meta_sig *>  nstr_vec;
        //Output function.
        tbb::concurrent_unordered_map<std::size_t,
            std::set<struct utils::meta_sig *> > output_fn;

        struct utils::meta_sig *n_str;

};


TEST_F(ACTireConcurrency, goto_function)
{




    goto_function<char, tbbscan::tbb_allocator> goto_fn;
    goto_fn.create_goto(&nstr_vec, output_fn);


    failure_function<char, tbbscan::tbb_allocator> failure_fn(goto_fn);
    failure_fn.create_failure(goto_fn, output_fn);

		
    tbbscan::actire_pe_engine<char, tbbscan::tbb_allocator> pe_parallel_search;
    tbbscan::result_callback<std::vector<std::string> > result(sig_key_vec);
    pe_parallel_search.search_parallel(goto_fn,
            failure_fn,
            output_fn,
            result,
            0,
            binary_hex_input.size(),
            file_name_binary_hex.c_str(),
            &binary_hex_input);
		
    //pe_parallel_search.search(&goto_fn, &failure_fn, &output_fn, result, &binary_hex_input);
/*
    std::string sigtype_code = utils::filetype_code_map(utils::pe_file);
    tbbscan::actire_engine_factory<char, tbbscan::tbb_allocator>::register_actire_type(sigtype_code,
            actire_pe_concurrency<char, tbbscan::tbb_allocator>::create);
*/
}


TEST_F(ACTireConcurrency, actire_engine_concurrency)
{
		
    //create engine support type per signature.
    //tbbscan::actire_sig_engine<char, tbbscan::tbb_allocator>  sig_engine;
    //EXPECT_TRUE(sig_engine.create_engine(nstr_vec, utils::pe_file));
		/*
    actire_pe_concurrency<char, tbbscan::tbb_allocator> pe_parallel_search;
    results_callback<std::vector<std::string> > result(sig_key_vec);
    pe_parallel_search.search_basic(&actire_engine.get_goto_fn(),
            &actire_engine.get_failure_fn(),
            &actire_engine.get_output_fn(),
            result,
						0,
						binary_hex_input.size(),
						file_name_binary_hex.c_str(),
            &binary_hex_input);
		*/
}
