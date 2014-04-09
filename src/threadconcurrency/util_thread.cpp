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
 * - util_thread supported mutex                                   R.chatsiri        24/03/2014
 */



// #include <thrift/thrift-config.h>

// #include <thrift/Thrift.h>
#include "utils/base/util_thread.hpp"

#define BASE_UNUSED_VARIABLE(x) ((void)(x))


#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#endif


namespace util{

int64_t util_thread::current_time_ticks(int64_t ticks_per_sec) {
  int64_t result;
  struct timeval now;
  int ret = BASE_GETTIMEOFDAY(&now, NULL);
  //assert(ret == 0);
  BASE_UNUSED_VARIABLE(ret); //squelching "unused variable" warning
  to_ticks(result, now, ticks_per_sec);
  return result;
}

}


