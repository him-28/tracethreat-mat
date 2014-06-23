#ifndef UTILS_GET_PROCESS_ID_NAME_HPP
#define UTILS_GET_PROCESS_ID_NAME_HPP

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

/*  Titles			                                 Authors	          Date
 * - Boost interprocesses (C) Copyright Ion Gaztanaga 2004-2012, Boost software
 *   license version 1.0
 * - support SHM-interprocesses                 R.Chatsiri         28/03/2014
 */



#include <boost/config.hpp>
#include <string>    //std::string
#include <sstream>   //std::stringstream
#include <boost/interprocess/detail/os_thread_functions.hpp>

namespace utils{

inline void get_process_id_name(std::string &str)
{
   std::stringstream sstr;
   sstr << "process_" << boost::interprocess::ipcdetail::get_current_process_id() << std::ends;
   str = sstr.str().c_str();
}

inline void get_process_id_ptr_name(std::string &str, const void *ptr)
{
   std::stringstream sstr;
   sstr << "process_" << boost::interprocess::ipcdetail::get_current_process_id() << "_" << ptr << std::ends;
   str = sstr.str().c_str();
}

inline const char *get_process_id_name()
{
   static std::string str;
   get_process_id_name(str);
   return str.c_str();
}

inline const char *get_process_id_ptr_name(void *ptr)
{
   static std::string str;
   get_process_id_ptr_name(str, ptr);
   return str.c_str();
}

inline const char *add_to_process_id_name(const char *name)
{
   static std::string str;
   get_process_id_name(str);
   str += name;
   return str.c_str();
}

inline const char *add_to_process_id_ptr_name(const char *name, void *ptr)
{
   static std::string str;
   get_process_id_ptr_name(str, ptr);
   str += name;
   return str.c_str();
}


}


#endif // UTILS_GET_PROCESS_ID_NAME_HPP
