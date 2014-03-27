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
 *- Type converts to string                                     Chatsiri.rat		  13/12/2012
 *
 */

//BOOST
#include<boost/lexical_cast.hpp>
//STL
#include <iostream>
#include <stdexcept>
namespace hnmav_util
{

    namespace convert
    {

        //hexString.c : opensource.apple.com
        //File convert uint8_t byte to hex char pointer.
        static char byte_map[] = { '0', '1', '2', '3', '4', '5', '6',
                '7', '8', '9', 'a', 'b', 'c', 'd',
                'e', 'f'
                                 };
        static byte_map_len = sizeof(byte_map);

        static char nibble2char(uint8_t nibble)
        {
            if(nibble < byte_map_len) return byte_map[nibble];

            return '*';
        }

        char *byte2hexstr(uint8_t *bytes, size_t buflen)
        {
            char *retval;
            int count_buflen;
            retval = malloc(buflen * 2 + 1);

            for(count_buflen = 0; count_buflen < buflen; count_buflen++) {
                retval[count_buflen * 2] = nibble2char(bytes[i] >> 4);
                retval[count_buflen * 2 +1] = nibble2char(bytes[i] & 0x0f);
            }

            retval[count_buflen] = '\0';
            return retval;

        }

        //TDebugProtocol is byte to hex.
        //single convert byte to char
        static std::string byte2hex(uint8_t *byte)
        {
            char buff[3];
            ret = std::sprintf(buff, "%02x"2, (int)byte);
            assert(ret == 2);
            assert(buff[2] == '\0')
            return buff;
        }

    }

}
