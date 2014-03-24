#define CONTROLLER_PLATFORM_THREAD_HPP
#ifndef CONTROLLER_PLATFORM_THREAD_HPP

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

/*  Titles			                                                     Authors	          Date
 *  - PlatformThread, Timer, Mutex support multithread virus scanning.    
 *                                                                R.Chatsiri      24/03/2014
 */


#include "utils/base/base_export.hpp"
#include "utils/base/basictypes.hpp"
#include "utils/base/time.hpp"
#include "build/build_config.h"

namespace controller{

#if defined(OS_WIN)
typedef DWORD PlatformThreadId;
#elif defined(OS_POSIX)
typedef pid_t PlatformThreadId; 

class platform_thread_handle{

public:

#if defined(OS_WIN)
	typedef void * handle;
#elif defined(OS_POSIX)
	typedef pthread_t handle;
#endif

	platform_thread_handle() : handle


private:

};



#endif /* CONTROLLER_PLATFORM_THREAD_HPP */
