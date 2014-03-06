/*
* Copyright 2014 MTSec, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*  Titles			                                                                  Authors	        Date
 *  -Platform OCL system initial from GPGPU                                       R.Chatsiri      26/02/2014
 */


#include "ocl/cl_bootstrap.hpp"
#include "data_structure/actire_parallel.hpp"

namespace kernel_ocl = hnmav_kernel;
namespace dstr       = data_structure;
class InitGPUPlatformTest : public ::testing::Test
{
    protected:

        virtual void SetUp() {
            opencl_file_path =  new std::string("/home/chatsiri/workspacecpp/pthread_sync_ocl/src/ocl/cl/struct_tire.h");

        }
        std::string *opencl_file_path;
};

TEST_F(InitGPUPlatformTest, load_system)
{
     kernel_ocl::cl_load_system<kernel_ocl::clutil_platform,
           dstr::dstr_def::work_groupitems,
           std::vector<boost::unordered_map<char, size_t> >,
           dstr::actire_parallel<char,
           											 size_t,
                                 boost::unordered_map<char, size_t>,
                                 std::vector<boost::unordered_map<char, size_t> > >
                                >	 load_system;

					load_system.set_opencl_file(*opencl_file_path);
						
					load_system.init_cl_system();
}

/*
TEST_F(InitGPUPlatformTest, platform_initial)
{



}
*/
