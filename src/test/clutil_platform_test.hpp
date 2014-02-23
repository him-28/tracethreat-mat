#include "ocl/utils/clutil_platform.hpp"

using namespace hnmav_kernel;

class InitGPUPlatformTest : public ::testing::Test
{
			protected:

			virtual void SetUp(){
					opencl_file_path =  new std::string("ocl/cl/trie_parallel.cl");

			}					
			string * opencl_file_path;
};

TEST_F(InitGPUPlatformTest, load_system)
{


}

TEST_F(InitGPUPlatformTest, platform_initial)
{
			


}
