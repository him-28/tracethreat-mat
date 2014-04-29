#ifndef DISASMS_DISASMPRIV_HPP
#define DISASMS_DISASMPRIV_HPP

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
 * - Sourcefire, Inc, GNU license.                              
 */

#include "decompiler/asm_x86/disasm_common.hpp"

namespace disasms
{
//    enum DIS_ACCESS;

    enum DIS_STATE {
        STATE_GETOP,
        STATE_CHECKDTYPE,
        STATE_CHECKSTYPE,
        STATE_DECODEX87,
        STATE_FINALIZE,
        STATE_COMPLETE,
        STATE_ERROR
    };

    struct DIS_ARGS {
        enum DIS_ACCESS access;
        enum DIS_SIZE size;
        enum X86REGS reg;
        union {
            uint8_t b;
            int8_t rb;
            uint16_t w;
            int16_t rw;
            uint32_t d;
            int32_t rd;
            /*    uint48_t f; FIXME */
            uint64_t q;
            int64_t rq;
            struct {
                enum X86REGS r1;  /* scaled */
                enum X86REGS r2;  /* added */
                uint8_t scale; /* r1 multiplier */
                int32_t disp;
            } marg;
        } arg;
    };

    /* FIXME: pack this thing and make macroes to access it in different compilers */
    struct DISASMED {
        uint16_t table_op;
        uint16_t real_op;
        enum DIS_STATE state;
        uint32_t opsize;
        uint32_t adsize;
        uint32_t segment;
        uint8_t cur;
        struct DIS_ARGS args[3];
    };

}


#endif
