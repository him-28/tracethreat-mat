#ifndef UTIL_THREAD_HPP
#define UTIL_THREAD_HPP 1

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
 * - Depend first version of thirft under license apache 2.0.
 * - util_thread supported matux class (pthread)                  R.Chatsiri            24/03/2014
 *
 */



#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

//#include <thrift/transport/PlatformSocket.h>

#include "utils/base/platformsocket.hpp"

namespace util{

/**
 * Utility methods
 *
 * This class contains basic utility methods for converting time formats,
 * and other common platform-dependent concurrency operations.
 * It should not be included in API headers for other concurrency library
 * headers, since it will, by definition, pull in all sorts of horrid
 * platform dependent stuff.  Rather it should be inluded directly in
 * concurrency library implementation source.
 *
 * @version $Id:$
 */
class util_thread{

  static const int64_t NS_PER_S = 1000000000LL;
  static const int64_t US_PER_S = 1000000LL;
  static const int64_t MS_PER_S = 1000LL;

  static const int64_t NS_PER_MS = NS_PER_S / MS_PER_S;
  static const int64_t NS_PER_US = NS_PER_S / US_PER_S;
  static const int64_t US_PER_MS = US_PER_S / MS_PER_S;

 public:

  /**
   * Converts millisecond timestamp into a BASE_TIMESPEC struct
   *
   * @param struct BASE_TIMESPEC& result
   * @param time or duration in milliseconds
   */
	static void to_time_spec(struct BASE_TIMESPEC& result, int64_t value) {
    result.tv_sec = value / MS_PER_S; // ms to s
    result.tv_nsec = (value % MS_PER_S) * NS_PER_MS; // ms to ns
  }

	
  static void to_time_val(struct timeval& result, int64_t value) {
    result.tv_sec  = static_cast<uint32_t>(value / MS_PER_S); // ms to s
    result.tv_usec = static_cast<uint32_t>((value % MS_PER_S) * US_PER_MS); // ms to us
  }

  static void to_ticks(int64_t& result, int64_t secs, int64_t old_ticks,
                      int64_t old_ticks_per_sec, int64_t new_tick_per_sec) {
    result = secs * new_tick_per_sec;
    result += old_ticks * new_tick_per_sec / old_ticks_per_sec;

    int64_t old_pernew = old_ticks_per_sec / new_tick_per_sec;
    if (old_pernew && ((old_ticks % oldPerNew) >= (old_pernew / 2))) {
      ++result;
    }
  }
  /**
   * Converts struct BASE_TIMESPEC to arbitrary-sized ticks since epoch
   */
  static void to_ticks(int64_t& result,
                      const struct BASE_TIMESPEC& value,
                      int64_t tick_per_sec) {
    return to_ticks(result, value.tv_sec, value.tv_nsec, NS_PER_S, tick_per_sec);
  }

  /**
   * Converts struct timeval to arbitrary-sized ticks since epoch
   */
  static void to_ticks(int64_t& result,
                      const struct timeval& value,
                      int64_t tick_per_sec) {
    return to_ticks(result, value.tv_sec, value.tv_usec, US_PER_S, tick_per_sec);
  }

  /**
   * Converts struct BASE_TIMESPEC to milliseconds
   */
  static void to_milliseconds(int64_t& result,
                             const struct BASE_TIMESPEC& value) {
    return to_ticks(result, value, MS_PER_S);
  }

  /**
   * Converts struct timeval to milliseconds
   */
  static void to_milliseconds(int64_t& result,
                             const struct timeval& value) {
    return to_ticks(result, value, MS_PER_S);
  }

  /**
   * Converts struct BASE_TIMESPEC to microseconds
   */
  static void to_usec(int64_t& result, const struct BASE_TIMESPEC& value) {
    return to_ticks(result, value, US_PER_S);
  }

  /**
   * Converts struct timeval to microseconds
   */
  static void to_usec(int64_t& result, const struct timeval& value) {
    return to_ticks(result, value, US_PER_S);
  }

  /**
   * Get current time as a number of arbitrary-size ticks from epoch
   */
  static int64_t current_time_ticks(int64_t tick_per_sec);

  /**
   * Get current time as milliseconds from epoch
   */
  static int64_t current_time() { return current_time_ticks(MS_PER_S); }

  /**
   * Get current time as micros from epoch
   */
  static int64_t current_time_usec() { return current_time_ticks(US_PER_S); }
};



}

#endif // #ifndef _BASE_CONCURRENCY_UTIL_H_
