#ifndef CONTROLLER_MONITOR_CONTROLLER_HPP
#define CONTROLLER_MONITOR_CONTROLLER_HPP

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
 * - Thrift apache version 2.0.
 * - monitor_controller support Thread_controller. Class created supports thread behaviors after 
 *   add thread and   runnable class to task.                                      
 *                                                                R.Chatsiri      24/03/2014
 * - Exception and mutex_controller_controller supported monitor_controller.
 *                                                                R.chatsiri      25/03/2014
 * - Rebuild code concept supports Factory class.                 R.chatsiri      25/03/2014
 */


//#include <thrift/concurrency/Exception.h>
#include "exception/system_exception.hpp"
#include "threadconcurrency/mutex_controller.hpp"
#include "utils/base/platform_socket.hpp"

//#include "utils/base/platform_socket.hpp"


#include <boost/utility.hpp>

namespace controller{

/*monitor */
/**
 * A monitor is a combination mutex and condition-event.  Waiting and
 * notifying condition events requires that the caller own the mutex.  mutex_controller
 * lock and unlock operations can be performed independently of condition
 * events.  This is more or less analogous to java.lang.Object multi-thread
 * operations.
 *
 * Note the monitor_controller can create a new, internal mutex; alternatively, a
 * separate mutex_controller can be passed in and the monitor_controller will re-use it without
 * taking ownership.  It's the user's responsibility to make sure that the
 * mutex_controller is not deallocated before the monitor_controller.
 *
 * Note that all methods are const.  monitor_controllers implement logical constness, not
 * bit constness.  This allows const methods to call monitor methods without
 * needing to cast away constness or change to non-const signatures.
 *
 * @version $Id:$
 */
class monitor_controller : boost::noncopyable {
 public:
  /** Creates a new mutex, and takes ownership of it. */
  monitor_controller();

  /** Uses the provided mutex without taking ownership. */
  explicit monitor_controller(mutex_controller* mutex);

  /** Uses the mutex inside the provided monitor_controller without taking ownership. */
  explicit monitor_controller(monitor_controller* monitor);

  /** Deallocates the mutex only if we own it. */
  virtual ~monitor_controller();

  mutex_controller& mutex() const;

  virtual void lock() const;

  virtual void unlock() const;

  /**
   * Waits a maximum of the specified timeout in milliseconds for the condition
   * to occur, or waits forever if timeout_ms == 0.
   *
   * Returns 0 if condition occurs, THRIFT_ETIMEDOUT on timeout, or an error code.
   */
  int wait_for_time_relative(int64_t timeout_ms) const;

  /**
   * Waits until the absolute time specified using struct THRIFT_TIMESPEC.
   * Returns 0 if condition occurs, THRIFT_ETIMEDOUT on timeout, or an error code.
   */
  int wait_for_time(const TIMESPEC* abstime) const;

  /**
   * Waits until the absolute time specified using struct timeval.
   * Returns 0 if condition occurs, THRIFT_ETIMEDOUT on timeout, or an error code.
   */
  int wait_for_time(const struct timeval* abstime) const;

  /**
   * Waits forever until the condition occurs.
   * Returns 0 if condition occurs, or an error code otherwise.
   */
  int wait_forever() const;

  /**
   * Exception-throwing version of wait_for_time_relative(), called simply
   * wait(int64) for historical reasons.  Timeout is in milliseconds.
   *
   * If the condition occurs,  this function returns cleanly; on timeout or
   * error an exception is thrown.
   */
  void wait(int64_t timeout_ms = 0LL) const;


  /** Wakes up one thread waiting on this monitor. */
  virtual void notify() const;

  /** Wakes up all waiting threads on this monitor. */
  virtual void notify_all() const;

 private:

  class impl;

  impl* impl_;
};

class synchronized {
 public:

 synchronized(const monitor_controller* monitor) : g(monitor->mutex()) { }
 synchronized(const monitor_controller& monitor) : g(monitor.mutex()) { }

 private:
  guard g;
};



/*end monitor */


}

#endif // #ifndef CONTROLLER_MONITOR_CONTROLLER_HPP 
