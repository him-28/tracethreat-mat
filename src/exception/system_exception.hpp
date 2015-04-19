#ifndef TRACE_EXCEPTION_SYSTEM_EXCEPTION
#define TRACE_EXCEPTION_SYSTEM_EXCEPTION

/*
* Copyright 2014 Chatsiri Rattana.
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

/*  Titles			                                          Authors	         Date

 *-Base exception class for Hnuman system                       Chatsiri.rat      18/11/2012
 *  --option system  exception                                  Chatsiri.rat      18/11/2012
 *  --cl utility exception                                      Chatsiri.rat      26/11/2012
 *  --Changed Base to System exception name                     Chatsiri.rat      26/11/2012
 *  --Separate sub type
 *         system : package system all
 *         cl     : opencl all
 *  --Add exception supported monitor class                     Chatsiri.rat      24/03/2014
 */

//BOOST
#include <boost/exception/all.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>

//OpenCL
#include <CL/cl.h>

// STL
#include <stdexcept>
#include <exception>
#include <string>
#include <iostream>

#include "exception/failure_handler_code.hpp"
// Internal code
#include "exception/exception_code.hpp"

namespace trace_exception
{
    using namespace boost;
    using namespace error;

    //________________________ Base virtual abstract ________________________________//
    class system_exception : public std::exception,  exception
    {
        public:
            virtual const char *what() const throw() = 0;
            virtual const char *process_name() const = 0;
    };

    namespace system
    {
        //_________________________  Option system Exception _____________________________//
        class option_system_exception : public system_exception
        {
            public:

                option_system_exception(std::string type) throw();

                const char *what() const throw();

                const char *process_name() const;
                // return   [error data , default file path]
                tuple<std::string, std::string> message_error_path();

                std::string default_path_system();

                ~option_system_exception() throw();

            private:
                std::string type_name;
        };

    }

    namespace cl
    {
        //_________________________  CL Boostraps ___________________________________//
        class clbootstrap_exception : public system_exception
        {
            public:
                clbootstrap_exception(std::string type) throw();
                ~clbootstrap_exception() throw();
            private:
                std::string type_name;
        };

        //_________________________  CL Exception ____________________________________//
        class clutil_exception : public std::runtime_error
        {
            public:
                clutil_exception(cl_int err, const std::string& name);

                std::string add_msg_err(cl_int err, const std::string& name);
        };
    }

    namespace file_system
    {

        class error_code_entry
        {

            public:
                error_code_entry();
                std::string get_error_cnumber(int  error_cnumber);
            private:
                int error_cnumber_;
        };

        class offset_exception : public system_exception
        {
            public:
                offset_exception(std::string type_name) throw();
                const char *what() const throw();
                const char *process_name() const;
                // Data structure size
                std::string message_error_file_is_null();
                ~offset_exception() throw();
            private:
                std::string type_name_;
                error_code_entry  error_codetype;

        };

    }

    namespace controller
    {

		class t_exception : public std::exception {
 public:
  t_exception():
    message_() {}

  t_exception(const std::string& message) :
    message_(message) {}

  virtual ~t_exception() throw() {}

  virtual const char* what() const throw() {
    if (message_.empty()) {
      return "Default TException.";
    } else {
      return message_.c_str();
    }
  }

 protected:
  std::string message_;

};
        class thread_exception : public std::exception
        {
            public:
                thread_exception():
                    message_() {}

                thread_exception(const std::string& message) :
                    message_(message) {}

                virtual ~thread_exception() throw() {}

                virtual const char *what() const throw() {
                    if (message_.empty()) {
                        return "Default thread_exception.";
                    } else {
                        return message_.c_str();
                    }
                }

            protected:
                std::string message_;

        };

        class no_such_task_exception : public thread_exception {};

        class uncancellable_task_exception : public thread_exception {};

        class invalid_argument_exception : public thread_exception {};

        class illegal_state_exception : public thread_exception
        {
            public:
                illegal_state_exception() {}
                illegal_state_exception(const std::string& message) : thread_exception(message) {}
        };

        class timed_out_exception : public thread_exception
        {
            public:
                timed_out_exception():thread_exception("TimedOutException") {};
                timed_out_exception(const std::string& message ) :
                    thread_exception(message) {}
        };

        class too_many_pending_tasks_exception : public thread_exception
        {
            public:
                too_many_pending_tasks_exception():thread_exception("TooManyPendingTasksException") {};
                too_many_pending_tasks_exception(const std::string& message ) :
                    thread_exception(message) {}
        };

        class system_resource_exception : public thread_exception
        {
            public:
                system_resource_exception() {}

                system_resource_exception(const std::string& message) :
                    thread_exception(message) {}
        };


    }

}

#endif /* TRACE_EXCEPTION_SYSTEM_EXCEPTION */
