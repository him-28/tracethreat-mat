#include "ocl/cl_bootstrap.hpp"


class InitGPUPlatformTest : public ::testing::Test
{
    protected:

        virtual void SetUp() {
            opencl_file_path =  new std::string("ocl/cl/trie_parallel.cl");

        }
        std::string *opencl_file_path;
};

TEST_F(InitGPUPlatformTest, load_system)
{
     kernel::cl_bootstrap::cl_load_system<kernel::clutil_platform,
           data_structure::dstr_def::work_groupitems,
           std::vector<boost::unordered_map<char, size_t> >,
           actire_parallel<char,
           size_t,
           boost::unordered_map<char, size_t>,
           std::vector<boost::unordered_map<char, size_t> > >
           >	 load_system(*opencl_file_path);
				
					load_system.init_cl_system();
}

/*
TEST_F(InitGPUPlatformTest, platform_initial)
{



}
*/
