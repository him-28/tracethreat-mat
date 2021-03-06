#define NUMBER_OF_BIT 8
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
#include <boost/lexical_cast.hpp>
//STL
#include <iostream>
#include <stdexcept>

#include "utils/base/system_code.hpp"

namespace utils
{

    namespace convert
    {

        //hexString.c : opensource.apple.com
        //File convert uint8_t byte to hex char pointer.
        static char byte_map[] = { '0', '1', '2', '3', '4', '5', '6',
                '7', '8', '9', 'a', 'b', 'c', 'd',
                'e', 'f'
                                 };
        static char byte_map_len = sizeof(byte_map);

        static char nibble2char(uint8_t nibble)
        {
            if(nibble < byte_map_len) return byte_map[nibble];

            return '*';
        }
        /*
        char *byte2hexstr(uint8_t *bytes, size_t buflen)
        {
        char *retval;
        int count_buflen;
        retval = (char*)malloc(buflen * 2 + 1);

        for(count_buflen = 0; count_buflen < buflen; count_buflen++) {
        retval[count_buflen * 2] = nibble2char(bytes[count_buflen] >> 4);
        retval[count_buflen * 2 +1] = nibble2char(bytes[count_buflen] & 0x0f);
        }

        retval[count_buflen] = '\0';
        return retval;

        }
        */
        char *byte2hexstr(uint8_t *bytes, size_t buflen)
        {
            int count_buflen;
            char *retval = (char *)malloc(sizeof(char) * buflen * 2 + 1);

            for(count_buflen = 0; count_buflen < buflen; count_buflen++) {
                retval[count_buflen * 2] = nibble2char(bytes[count_buflen] >> 4);
                retval[count_buflen * 2 +1] = nibble2char(bytes[count_buflen] & 0x0f);
            }

            retval[count_buflen] = '\0';
            return retval;

        }

        //TDebugProtocol is byte to hex.
        //single convert byte to char
        static std::string byte2hex(const uint8_t byte)
        {
            char buff[3];
            int ret = std::sprintf(buff, "%02x", (int)byte);
            assert(ret == 2);
            assert(buff[2] == '\0');
            return buff;
        }

        //Convert int64_t and char: http://goo.gl/WSstZX
        static void int64_to_char(char *char_value, int64_t number)
        {
            memcpy(char_value, &number, NUMBER_OF_BIT);
        }

        static int64_t char_to_int64bit(const char *char_value)
        {
            int64_t number = 0;
            memcpy(&number, char_value, NUMBER_OF_BIT);
            return number;
        }


    }

}
