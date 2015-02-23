#include "tbbscan/data_structure/actire_concurrency.hpp"
#include "tbb/cache_aligned_allocator.h"
#include "tbb/concurrent_vector.h"

#include <boost/assign/std/vector.hpp>
#include <boost/unordered_map.hpp>

#include "utils/file_offset_handler.hpp"

#include "scan/file_scan_policy.hpp"


#include "utils/base/system_code.hpp"

//#include "utils/base/common.hpp"
#include "utils/base/common.hpp"

#include "utils/convert.hpp"


using namespace utils;
using tbbscan::goto_function;
using tbbscan::failure_function;

//using tbbscan::actire_pe_engine;
//using tbbscan::result_callback;


class ACTireConcurrency : public ::testing::Test
{
		public :
    typedef message_tracethreat::InfectedFileInfo  threatinfo_type;

    typedef	std::vector<threatinfo_type *>  rcb_container_type;

    protected:


        virtual void SetUp() {


            std::string sig1 = "09cd21b80";
            std::string sig2 = "09cd21b44ce1aea";

            file_name_binary_hex = "trojan_test.exe";
            char *binary_hex  = "ab4c5d23434e08d3d211d890042004230801b231e1e7c4d5a50000200000004000f00ffff0000b80000000000000040001a000000ab4353453453b123e3288b";
					  std::string binary_hex_str(binary_hex);

						binary_hex_input.resize(binary_hex_str.size());
			
						std::transform(binary_hex_str.begin(), binary_hex_str.end(),
                           binary_hex_input.begin(),
                           [](char c){
                              return c;
													 });
				
						/*
            memcpy(&binary_hex_input[*binary_hex_input.grow_by(strlen(binary_hex))],
                    binary_hex,
                    strlen(binary_hex)+1);
					  */

						binary_hex_input.resize(strlen(binary_hex));

            nstr_vec.push_back(new struct utils::meta_sig);
            n_str = nstr_vec[0];
            n_str->sig  = "09cd21b80"; //sig_node to sig
            n_str->virname   = "test_trojan_a";
            n_str->sig_type  = utils::pe_file;
            n_str->sig_size  = strlen(n_str->sig);
            sig_key_vec.push_back(std::string(n_str->sig));

            nstr_vec.push_back(new struct utils::meta_sig);
            n_str = nstr_vec[1];
            n_str->sig  = "09cd21b44ce1aeadaca1e4bc647c46d0dd553e637b06cc2";
            n_str->virname   = "test_trojan_b";
            n_str->sig_type  = utils::pe_file;
            n_str->sig_size  = strlen(n_str->sig);
            sig_key_vec.push_back(std::string(n_str->sig));

            nstr_vec.push_back(new struct utils::meta_sig);
            n_str = nstr_vec[2];
            n_str->sig  = "4d5a50000200000004000f00ffff0000b80000000000000040001a000000";
            n_str->virname   = "trojan-zbot-15693";
            n_str->sig_type  = utils::pe_file;
            n_str->sig_size  = strlen(n_str->sig);
            sig_key_vec.push_back(std::string(n_str->sig));

            nstr_vec.push_back(new struct utils::meta_sig);
            n_str = nstr_vec[3];
            n_str->sig  = "2353ab1e119d32c67a23ab3d";
            n_str->virname   = "test_trojan_d";
            n_str->sig_type  = utils::pe_file;
            n_str->sig_size  = strlen(n_str->sig);
            sig_key_vec.push_back(std::string(n_str->sig));

        }

        std::string file_name_binary_hex;

        std::vector<std::string> sig_key_vec;
        //binary input from file (Test only)
        //tbb::concurrent_vector<char> binary_hex_input;
        std::vector<char> binary_hex_input;

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
    tbbscan::result_callback<rcb_container_type, threatinfo_type > result;
    pe_parallel_search.search_parallel(goto_fn,
            failure_fn,
            output_fn,
            result,
            0,
            binary_hex_input.size(),
            file_name_binary_hex.c_str(),
            &binary_hex_input);


    std::string sigtype_code = utils::filetype_code_map(utils::pe_file);
    tbbscan::actire_engine_factory<char, tbbscan::tbb_allocator>::register_actire_type(sigtype_code,
            tbbscan::actire_pe_engine<char, tbbscan::tbb_allocator>::create);

}


TEST_F(ACTireConcurrency, actire_engine_concurrency)
{

    //create engine support type per signature.
    tbbscan::actire_sig_engine<char, tbbscan::tbb_allocator>  sig_engine;
    EXPECT_TRUE(sig_engine.create_engine(nstr_vec, utils::pe_file));
    //Create pe scan file engine.
    tbbscan::actire_pe_engine<char, tbbscan::tbb_allocator> pe_parallel;
    //Call back support find index found virus.
    //tbbscan::result_callback<std::vector<std::string> > result;
    tbbscan::result_callback<rcb_container_type, threatinfo_type > result;

    //Search engine of pe opreate with binary stream match signature of virus.
    pe_parallel.search_parallel(sig_engine.get_goto_fn(),
            sig_engine.get_failure_fn(),
            sig_engine.get_output_fn(),
            result,
            0,
            binary_hex_input.size(),
            file_name_binary_hex.c_str(),
            &binary_hex_input);

}
