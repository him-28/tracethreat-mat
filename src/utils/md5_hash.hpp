#ifndef UTILS_MD5_HASH_HPP
#define UTILS_MD5_HASH_HPP

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
 * - Copyright 2010,  Zihan Liu, (updogliu@tencent.com) GNU Distributed license.
 *   http://sourceforge.net/projects/libmd5-rfc/files/
 *                                               R.Chatsiri   25/03/2014
 */



#include <string>

#include "utils/base/common.hpp"
namespace utils
{

    namespace convert
    {

        uint64_t md5_hash(const unsigned char *s, const unsigned int len);
        uint64_t md5_hash(const std::string& s);

    }

}
#endif  // UTILS_MD5_HASH_HPP
