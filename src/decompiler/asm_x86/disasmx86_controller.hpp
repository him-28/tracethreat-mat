#ifndef  DISASMX86_CONTROLLER_HPP
#define  DISASMX86_CONTROLLER_HPP

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

#include <assert.h>
#include <stdio.h>
#include <string.h>

namespace disasm
{

    enum ADDRS {
        ADDR_REG_EAX,
        ADDR_REG_ECX,
        ADDR_REG_EDX,
        ADDR_REG_EBX,
        ADDR_REG_ESP,
        ADDR_REG_EBP,
        ADDR_REG_ESI,
        ADDR_REG_EDI,
        ADDR_REG_ES,
        ADDR_REG_CS,
        ADDR_REG_SS,
        ADDR_REG_DS,
        ADDR_REG_FS,
        ADDR_REG_GS,
        ADDR_MRM_GEN,
        ADDR_OFFSET,
        ADDR_MRM_GEN_EG,
        ADDR_MRM_GEN_GE,
        ADDR_MRM_GEN_GM,
        ADDR_MRM_GEN_ES,
        ADDR_MRM_GEN_SE,
        ADDR_MRM_EXTRA_1A,
        ADDR_MRM_EXTRA_1A_M,
        ADDR_MRM_GEN_RC,
        ADDR_MRM_GEN_RD,
        ADDR_MRM_GEN_CR,
        ADDR_MRM_GEN_DR,
        ADDR_IMMED,
        ADDR_RELJ,
        ADDR_IMPLICIT,
        ADDR_NOADDR
    };

    enum ASIZE {
        SIZE_BYTE,
        SIZE_BYTEH,
        SIZE_WORD,
        SIZE_DWORD,
        SIZE_QWORD,
        SIZE_WD,
        SIZE_DF,
        SIZE_NOSIZE
    };

    static const uint8_t sizemap[SIZE_NOSIZE+1][2] = {
        {1,1},       /* SIZE_BYTE */
        {255,255},   /* SIZE_BYTEH */
        {2,2},       /* SIZE_WORD */
        {4,4},       /* SIZE_DWORD */
        {255,255},   /* SIZE_QWORD */
        {4,2},       /* SIZE_WD */
        {6,4},       /* SIZE_DF */
        {255,255}    /* SIZE_NOSIZE */
    };

    static const uint8_t regmap[SIZE_DWORD+1][ADDR_REG_GS+1] = {
        /* SIZE_BYTE */
        {REG_AL, REG_CL, REG_DL, REG_BL, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID},
        /* SIZE_BYTEH */
        {REG_AH, REG_CH, REG_DH, REG_BH, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID},
        /* SIZE_WORD */
        {REG_AX, REG_CX, REG_DX, REG_BX, REG_SP, REG_BP, REG_SI, REG_DI, REG_ES, REG_CS, REG_SS, REG_DS, REG_FS, REG_GS},
        /* SIZE_DWORD */
        {REG_EAX, REG_ECX, REG_EDX, REG_EBX, REG_ESP, REG_EBP, REG_ESI, REG_EDI, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID}
    };

    static const uint8_t mrm_regmap[3][8] = {
        /* SIZEB */
        {REG_AL, REG_CL, REG_DL, REG_BL, REG_AH, REG_CH, REG_DH, REG_BH},
        /* SIZEW */
        {REG_AX, REG_CX, REG_DX, REG_BX, REG_SP, REG_BP, REG_SI, REG_DI},
        /* SIZED */
        {REG_EAX, REG_ECX, REG_EDX, REG_EBX, REG_ESP, REG_EBP, REG_ESI, REG_EDI}
    };

    static const uint8_t mrm_sregmap[3][8] = {
        /* SIZEB */
        {REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID},
        /* SIZEW */
        {REG_ES, REG_CS, REG_SS, REG_DS, REG_FS, REG_GS, REG_INVALID, REG_INVALID},
        /* SIZED */
        {REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID}
    };

    static const uint8_t mrm_cregmap[3][8] = {
        /* SIZEB */
        {REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID},
        /* SIZEW */
        {REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID},
        /* SIZED */
        {REG_CR0, REG_INVALID, REG_CR2, REG_CR3, REG_CR4, REG_INVALID, REG_INVALID, REG_INVALID}
    };

    static const uint8_t mrm_dregmap[3][8] = {
        /* SIZEB */
        {REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID},
        /* SIZEW */
        {REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID, REG_INVALID},
        /* SIZED */
        {REG_DR0, REG_DR1, REG_DR2, REG_DR3, REG_INVALID, REG_INVALID, REG_DR6, REG_DR7}
    };

    static const struct {
        enum X86REGS r1;
        enum X86REGS r2;
    } mrm_regmapw[8] = {
        {REG_BX, REG_SI}, {REG_BX, REG_DI}, {REG_BP, REG_SI}, {REG_BP, REG_DI}, {REG_SI, REG_INVALID}, {REG_DI, REG_INVALID}, {REG_BP, REG_INVALID}, {REG_BX, REG_INVALID}
    };

    static const struct {
        enum X86OPS op;
        enum DIS_SIZE size;
    } x87_mrm[8][8] = {
        /* D8 */
        {{OP_FADD,SIZED},{OP_FMUL,SIZED},{OP_FCOM,SIZED},{OP_FCOMP,SIZED},{OP_FSUB,SIZED},{OP_FSUBR,SIZED},{OP_FDIV,SIZED},{OP_FDIVR,SIZED}},
        /* D9 */
        {{OP_FLD,SIZED},{OP_INVALID,SIZED},{OP_FST,SIZED},{OP_FSTP,SIZED},{OP_FLDENV,SIZEPTR},{OP_FLDCW,SIZEW},{OP_FSTENV,SIZEPTR},{OP_FSTCW,SIZEW}},
        /* DA */
        {{OP_FIADD,SIZED},{OP_FIMUL,SIZED},{OP_FICOM,SIZED},{OP_FICOMP,SIZED},{OP_FISUB,SIZED},{OP_FISUBR,SIZED},{OP_FIDIV,SIZED},{OP_FIDIVR,SIZED}},
        /* DB */
        {{OP_FILD,SIZED},{OP_FISTTP,SIZED},{OP_FIST,SIZED},{OP_FISTP,SIZED},{OP_INVALID,SIZED},{OP_FLD,SIZET},{OP_INVALID,SIZED},{OP_FSTP,SIZET} },
        /* DC */
        {{OP_FADD,SIZEQ},{OP_FMUL,SIZEQ},{OP_FCOM,SIZEQ},{OP_FCOMP,SIZEQ},{OP_FSUB,SIZEQ},{OP_FSUBR,SIZEQ},{OP_FDIV,SIZEQ},{OP_FDIVR,SIZEQ}},
        /* DD */
        {{OP_FLD,SIZEQ},{OP_FISTTP,SIZEQ},{OP_FST,SIZEQ},{OP_FSTP,SIZEQ},{OP_FRSTOR,SIZEPTR},{OP_INVALID,SIZED},{OP_FSAVE,SIZEPTR},{OP_FSTSW,SIZEW}},
        /* DE */
        {{OP_FIADD,SIZEW},{OP_FIMUL,SIZEW},{OP_FICOM,SIZEW},{OP_FICOMP,SIZEW},{OP_FISUB,SIZEW},{OP_FISUBR,SIZEW},{OP_FIDIV,SIZEW},{OP_FIDIVR,SIZEW}},
        /* DF */
        {{OP_FILD,SIZEW},{OP_FISTTP,SIZEW},{OP_FIST,SIZEW},{OP_FISTP,SIZEW},{OP_FBLD,SIZET},{OP_FILD,SIZEQ},{OP_FBSTP,SIZET},{OP_FISTP,SIZEQ}}
    };

    static const struct {
        enum X86OPS op;
        enum { X87_NONE, X87_ONE, X87_S, X87_R } args;
    } x87_st[8][64] = {
        /* D8 */
        {
            {OP_FADD,X87_S},{OP_FADD,X87_S},{OP_FADD,X87_S},{OP_FADD,X87_S},{OP_FADD,X87_S},{OP_FADD,X87_S},{OP_FADD,X87_S},{OP_FADD,X87_S},
            {OP_FMUL,X87_S},{OP_FMUL,X87_S},{OP_FMUL,X87_S},{OP_FMUL,X87_S},{OP_FMUL,X87_S},{OP_FMUL,X87_S},{OP_FMUL,X87_S},{OP_FMUL,X87_S},
            {OP_FCOM,X87_S},{OP_FCOM,X87_S},{OP_FCOM,X87_S},{OP_FCOM,X87_S},{OP_FCOM,X87_S},{OP_FCOM,X87_S},{OP_FCOM,X87_S},{OP_FCOM,X87_S},
            {OP_FCOMP,X87_S},{OP_FCOMP,X87_S},{OP_FCOMP,X87_S},{OP_FCOMP,X87_S},{OP_FCOMP,X87_S},{OP_FCOMP,X87_S},{OP_FCOMP,X87_S},{OP_FCOMP,X87_S},
            {OP_FSUB,X87_S},{OP_FSUB,X87_S},{OP_FSUB,X87_S},{OP_FSUB,X87_S},{OP_FSUB,X87_S},{OP_FSUB,X87_S},{OP_FSUB,X87_S},{OP_FSUB,X87_S},
            {OP_FSUBR,X87_S},{OP_FSUBR,X87_S},{OP_FSUBR,X87_S},{OP_FSUBR,X87_S},{OP_FSUBR,X87_S},{OP_FSUBR,X87_S},{OP_FSUBR,X87_S},{OP_FSUBR,X87_S},
            {OP_FDIV,X87_S},{OP_FDIV,X87_S},{OP_FDIV,X87_S},{OP_FDIV,X87_S},{OP_FDIV,X87_S},{OP_FDIV,X87_S},{OP_FDIV,X87_S},{OP_FDIV,X87_S},
            {OP_FDIVR,X87_S},{OP_FDIVR,X87_S},{OP_FDIVR,X87_S},{OP_FDIVR,X87_S},{OP_FDIVR,X87_S},{OP_FDIVR,X87_S},{OP_FDIVR,X87_S},{OP_FDIVR,X87_S}
        },

        /* D9 */
        {
            {OP_FLD,X87_S},{OP_FLD,X87_S},{OP_FLD,X87_S},{OP_FLD,X87_S},{OP_FLD,X87_S},{OP_FLD,X87_S},{OP_FLD,X87_S},{OP_FLD,X87_S},
            {OP_FXCH,X87_S},{OP_FXCH,X87_S},{OP_FXCH,X87_S},{OP_FXCH,X87_S},{OP_FXCH,X87_S},{OP_FXCH,X87_S},{OP_FXCH,X87_S},{OP_FXCH,X87_S},
            {OP_FNOP,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_FCHS,X87_NONE},{OP_FABS,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_FTST,X87_NONE},{OP_FXAM,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_FLD1,X87_NONE},{OP_FLDL2T,X87_NONE},{OP_FLDL2E,X87_NONE},{OP_FLDPI,X87_NONE},{OP_FLDLG2,X87_NONE},{OP_FLDLN2,X87_NONE},{OP_FLDZ,X87_S},{OP_INVALID,X87_NONE},
            {OP_F2XM1,X87_NONE},{OP_FYL2X,X87_NONE},{OP_FPTAN,X87_NONE},{OP_FPATAN,X87_NONE},{OP_FXTRACT,X87_NONE},{OP_FPREM1,X87_NONE},{OP_FDECSTP,X87_NONE},{OP_FINCSTP,X87_NONE},
            {OP_FPREM,X87_NONE},{OP_FYL2XP1,X87_NONE},{OP_FSQRT,X87_NONE},{OP_FSINCOS,X87_NONE},{OP_FRNDINT,X87_NONE},{OP_FSCALE,X87_NONE},{OP_FSIN,X87_NONE},{OP_FCOS,X87_NONE}
        },

        /* DA */
        {
            {OP_FCMOVB,X87_S},{OP_FCMOVB,X87_S},{OP_FCMOVB,X87_S},{OP_FCMOVB,X87_S},{OP_FCMOVB,X87_S},{OP_FCMOVB,X87_S},{OP_FCMOVB,X87_S},{OP_FCMOVB,X87_S},
            {OP_FCMOVE,X87_S},{OP_FCMOVE,X87_S},{OP_FCMOVE,X87_S},{OP_FCMOVE,X87_S},{OP_FCMOVE,X87_S},{OP_FCMOVE,X87_S},{OP_FCMOVE,X87_S},{OP_FCMOVE,X87_S},
            {OP_FCMOVBE,X87_S},{OP_FCMOVBE,X87_S},{OP_FCMOVBE,X87_S},{OP_FCMOVBE,X87_S},{OP_FCMOVBE,X87_S},{OP_FCMOVBE,X87_S},{OP_FCMOVBE,X87_S},{OP_FCMOVBE,X87_S},
            {OP_FCMOVU,X87_S},{OP_FCMOVU,X87_S},{OP_FCMOVU,X87_S},{OP_FCMOVU,X87_S},{OP_FCMOVU,X87_S},{OP_FCMOVU,X87_S},{OP_FCMOVU,X87_S},{OP_FCMOVU,X87_S},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_INVALID,X87_NONE},{OP_FUCOMPP,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE}
        },

        /* DB */
        {
            {OP_FCMOVNB,X87_S},{OP_FCMOVNB,X87_S},{OP_FCMOVNB,X87_S},{OP_FCMOVNB,X87_S},{OP_FCMOVNB,X87_S},{OP_FCMOVNB,X87_S},{OP_FCMOVNB,X87_S},{OP_FCMOVNB,X87_S},
            {OP_FCMOVNE,X87_S},{OP_FCMOVNE,X87_S},{OP_FCMOVNE,X87_S},{OP_FCMOVNE,X87_S},{OP_FCMOVNE,X87_S},{OP_FCMOVNE,X87_S},{OP_FCMOVNE,X87_S},{OP_FCMOVNE,X87_S},
            {OP_FCMOVNBE,X87_S},{OP_FCMOVNBE,X87_S},{OP_FCMOVNBE,X87_S},{OP_FCMOVNBE,X87_S},{OP_FCMOVNBE,X87_S},{OP_FCMOVNBE,X87_S},{OP_FCMOVNBE,X87_S},{OP_FCMOVNBE,X87_S},
            {OP_FCMOVNU,X87_S},{OP_FCMOVNU,X87_S},{OP_FCMOVNU,X87_S},{OP_FCMOVNU,X87_S},{OP_FCMOVNU,X87_S},{OP_FCMOVNU,X87_S},{OP_FCMOVNU,X87_S},{OP_FCMOVNU,X87_S},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_FCLEX,X87_NONE},{OP_FINIT,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_FUCOMI,X87_S},{OP_FUCOMI,X87_S},{OP_FUCOMI,X87_S},{OP_FUCOMI,X87_S},{OP_FUCOMI,X87_S},{OP_FUCOMI,X87_S},{OP_FUCOMI,X87_S},{OP_FUCOMI,X87_S},
            {OP_FCOMI,X87_S},{OP_FCOMI,X87_S},{OP_FCOMI,X87_S},{OP_FCOMI,X87_S},{OP_FCOMI,X87_S},{OP_FCOMI,X87_S},{OP_FCOMI,X87_S},{OP_FCOMI,X87_S},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE}
        },

        /* DC */
        {
            {OP_FADD,X87_R},{OP_FADD,X87_R},{OP_FADD,X87_R},{OP_FADD,X87_R},{OP_FADD,X87_R},{OP_FADD,X87_R},{OP_FADD,X87_R},{OP_FADD,X87_R},
            {OP_FMUL,X87_R},{OP_FMUL,X87_R},{OP_FMUL,X87_R},{OP_FMUL,X87_R},{OP_FMUL,X87_R},{OP_FMUL,X87_R},{OP_FMUL,X87_R},{OP_FMUL,X87_R},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_FSUBR,X87_R},{OP_FSUBR,X87_R},{OP_FSUBR,X87_R},{OP_FSUBR,X87_R},{OP_FSUBR,X87_R},{OP_FSUBR,X87_R},{OP_FSUBR,X87_R},{OP_FSUBR,X87_R},
            {OP_FSUB,X87_R},{OP_FSUB,X87_R},{OP_FSUB,X87_R},{OP_FSUB,X87_R},{OP_FSUB,X87_R},{OP_FSUB,X87_R},{OP_FSUB,X87_R},{OP_FSUB,X87_R},
            {OP_FDIVR,X87_R},{OP_FDIVR,X87_R},{OP_FDIVR,X87_R},{OP_FDIVR,X87_R},{OP_FDIVR,X87_R},{OP_FDIVR,X87_R},{OP_FDIVR,X87_R},{OP_FDIVR,X87_R},
            {OP_FDIV,X87_R},{OP_FDIV,X87_R},{OP_FDIV,X87_R},{OP_FDIV,X87_R},{OP_FDIV,X87_R},{OP_FDIV,X87_R},{OP_FDIV,X87_R},{OP_FDIV,X87_R}
        },

        /* DD */
        {
            {OP_FFREE,X87_ONE},{OP_FFREE,X87_ONE},{OP_FFREE,X87_ONE},{OP_FFREE,X87_ONE},{OP_FFREE,X87_ONE},{OP_FFREE,X87_ONE},{OP_FFREE,X87_ONE},{OP_FFREE,X87_ONE},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_FST,X87_ONE},{OP_FST,X87_ONE},{OP_FST,X87_ONE},{OP_FST,X87_ONE},{OP_FST,X87_ONE},{OP_FST,X87_ONE},{OP_FST,X87_ONE},{OP_FST,X87_ONE},
            {OP_FSTP,X87_ONE},{OP_FSTP,X87_ONE},{OP_FSTP,X87_ONE},{OP_FSTP,X87_ONE},{OP_FSTP,X87_ONE},{OP_FSTP,X87_ONE},{OP_FSTP,X87_ONE},{OP_FSTP,X87_ONE},
            {OP_FUCOM,X87_R},{OP_FUCOM,X87_R},{OP_FUCOM,X87_R},{OP_FUCOM,X87_R},{OP_FUCOM,X87_R},{OP_FUCOM,X87_R},{OP_FUCOM,X87_R},{OP_FUCOM,X87_R},
            {OP_FUCOMP,X87_R},{OP_FUCOMP,X87_R},{OP_FUCOMP,X87_R},{OP_FUCOMP,X87_R},{OP_FUCOMP,X87_R},{OP_FUCOMP,X87_R},{OP_FUCOMP,X87_R},{OP_FUCOMP,X87_R},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE}
        },

        /* DE */
        {
            {OP_FADDP,X87_R},{OP_FADDP,X87_R},{OP_FADDP,X87_R},{OP_FADDP,X87_R},{OP_FADDP,X87_R},{OP_FADDP,X87_R},{OP_FADDP,X87_R},{OP_FADDP,X87_R},
            {OP_FMULP,X87_R},{OP_FMULP,X87_R},{OP_FMULP,X87_R},{OP_FMULP,X87_R},{OP_FMULP,X87_R},{OP_FMULP,X87_R},{OP_FMULP,X87_R},{OP_FMULP,X87_R},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_INVALID,X87_NONE},{OP_FCOMPP,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_FSUBRP,X87_R},{OP_FSUBRP,X87_R},{OP_FSUBRP,X87_R},{OP_FSUBRP,X87_R},{OP_FSUBRP,X87_R},{OP_FSUBRP,X87_R},{OP_FSUBRP,X87_R},{OP_FSUBRP,X87_R},
            {OP_FSUBP,X87_R},{OP_FSUBP,X87_R},{OP_FSUBP,X87_R},{OP_FSUBP,X87_R},{OP_FSUBP,X87_R},{OP_FSUBP,X87_R},{OP_FSUBP,X87_R},{OP_FSUBP,X87_R},
            {OP_FDIVRP,X87_R},{OP_FDIVRP,X87_R},{OP_FDIVRP,X87_R},{OP_FDIVRP,X87_R},{OP_FDIVRP,X87_R},{OP_FDIVRP,X87_R},{OP_FDIVRP,X87_R},{OP_FDIVRP,X87_R},
            {OP_FDIVP,X87_R},{OP_FDIVP,X87_R},{OP_FDIVP,X87_R},{OP_FDIVP,X87_R},{OP_FDIVP,X87_R},{OP_FDIVP,X87_R},{OP_FDIVP,X87_R},{OP_FDIVP,X87_R}
        },

        /* DF */
        {
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_INVALID,X87_NONE},{OP_FSTSW,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},
            {OP_FUCOMIP,X87_S},{OP_FUCOMIP,X87_S},{OP_FUCOMIP,X87_S},{OP_FUCOMIP,X87_S},{OP_FUCOMIP,X87_S},{OP_FUCOMIP,X87_S},{OP_FUCOMIP,X87_S},{OP_FUCOMIP,X87_S},
            {OP_FCOMIP,X87_S},{OP_FCOMIP,X87_S},{OP_FCOMIP,X87_S},{OP_FCOMIP,X87_S},{OP_FCOMIP,X87_S},{OP_FCOMIP,X87_S},{OP_FCOMIP,X87_S},{OP_FCOMIP,X87_S},
            {OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE},{OP_INVALID,X87_NONE}
        }
    };


    static const char *mnemonic[] = {
        "Invalid opcode",
        "aaa",
        "aam",
        "aad",
        "aas",
        "add",
        "adc",
        "and",
        "arpl",
        "bound",
        "bsf",
        "bsr",
        "bswap",
        "bt",
        "btc",
        "btr",
        "bts",
        "call",
        "cdq",
        "cwd",
        "cwde",
        "cbw",
        "clc",
        "cld",
        "cli",
        "clts",
        "cmc",
        "cmovo",
        "cmovno",
        "cmovc",
        "cmovnc",
        "cmovz",
        "cmovnz",
        "cmovbe",
        "cmova",
        "cmovs",
        "cmovns",
        "cmovp",
        "cmovnp",
        "cmovl",
        "cmovge",
        "cmovle",
        "cmovg",
        "cmp",
        "cmpsd",
        "cmpsw",
        "cmpsb",
        "cmpxchg",
        "cmpxchg8b",
        "cpuid",
        "daa",
        "das",
        "dec",
        "div",
        "enter",
        "fwait",
        "hlt",
        "idiv",
        "imul",
        "inc",
        "in",
        "insd",
        "insw",
        "insb",
        "int",
        "int3",
        "into",
        "invd",
        "invlpg",
        "iret",
        "jo",
        "jno",
        "jc",
        "jnc",
        "jz",
        "jnz",
        "jbe",
        "ja",
        "js",
        "jns",
        "jp",
        "jnp",
        "jl",
        "jge",
        "jle",
        "jg",
        "jmp",
        "lahf",
        "lar",
        "lds",
        "les",
        "lfs",
        "lgs",
        "lea",
        "leave",
        "lgdt",
        "lidt",
        "lldt",
        "lock",
        "lodsd",
        "lodsw",
        "lodsb",
        "loop",
        "loope",
        "loopne",
        "jecxz",
        "lsl",
        "lss",
        "ltr",
        "mov",
        "movsd",
        "movsw",
        "movsb",
        "movsx",
        "movzx",
        "mul",
        "neg",
        "nop",
        "not",
        "or",
        "out",
        "outsd",
        "outsw",
        "outsb",
        "push",
        "pushad",
        "pusha",
        "pushfd",
        "pushf",
        "pop",
        "popad",
        "popfd",
        "popf",
        "rcl",
        "rcr",
        "rdmsr",
        "rdpmc",
        "rdtsc",
        "repe",
        "repne",
        "retf",
        "retn",
        "rol",
        "ror",
        "rsm",
        "sahf",
        "sar",
        "sbb",
        "scasd",
        "scasw",
        "scasb",
        "seto",
        "setno",
        "setc",
        "setnc",
        "setz",
        "setnz",
        "setbe",
        "seta",
        "sets",
        "setns",
        "setp",
        "setnp",
        "setl",
        "setge",
        "setle",
        "setg",
        "sgdt",
        "sidt",
        "shl",
        "shld",
        "shr",
        "shrd",
        "sldt",
        "stosd",
        "stosw",
        "stosb",
        "str",
        "stc",
        "std",
        "sti",
        "sub",
        "syscall",
        "sysenter",
        "sysexit",
        "sysret",
        "test",
        "ud2",
        "verr",
        "verrw",
        "wbinvd",
        "wrmsr",
        "xadd",
        "xchg",
        "xlat",
        "xor",
        "Operand Size",
        "Address Size",
        "Segment Override",
        "2byte escape",
        "FPU escape",

        "f2xm1",
        "fabs",
        "fadd",
        "faddp",
        "fbld",
        "fbstp",
        "fchs",
        "fclex",
        "fcmovb",
        "fcmovbe",
        "fcmove",
        "fcmovnb",
        "fcmovnbe",
        "fcmovne",
        "fcmovnu",
        "fcmovu",
        "fcom",
        "fcomi",
        "fcomip",
        "fcomp",
        "fcompp",
        "fcos",
        "fdecstp",
        "fdiv",
        "fdivp",
        "fdivr",
        "fdivrp",
        "ffree",
        "fiadd",
        "ficom",
        "ficomp",
        "fidiv",
        "fidivr",
        "fild",
        "fimul",
        "fincstp",
        "finit",
        "fist",
        "fistp",
        "fisttp",
        "fisub",
        "fisubr",
        "fld",
        "fld1",
        "fldcw",
        "fldenv",
        "fldl2e",
        "fldl2t",
        "fldlg2",
        "fldln2",
        "fldpi",
        "fldz",
        "fmul",
        "fmulp",
        "fnop",
        "fpatan",
        "fprem",
        "fprem1",
        "fptan",
        "frndint",
        "frstor",
        "fscale",
        "fsin",
        "fsincos",
        "fsqrt",
        "fsave",
        "fst",
        "fstcw",
        "fstenv",
        "fstp",
        "fstsw",
        "fsub",
        "fsubp",
        "fsubr",
        "fsubrp",
        "ftst",
        "fucom",
        "fucomi",
        "fucomip",
        "fucomp",
        "fucompp",
        "fxam",
        "fxch",
        "fxtract",
        "fyl2x",
        "fyl2xp1"
    };

    struct OPCODES {
        enum ADDRS dmethod;
        enum ASIZE dsize;
        enum ADDRS smethod;
        enum ASIZE ssize;
        enum X86OPS op;
    };

    static const struct {
        enum X86OPS op;
        enum { ADDSZ_ZERO, ADDSZ_ONE } addsz;
    } extra_1a[][8] = {
        /* 8f - pop */
        {{OP_POP,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}},
        /* 80-83 - GRP1 */
        {{OP_ADD,0}, {OP_OR,0}, {OP_ADC,0}, {OP_SBB,0}, {OP_AND,0}, {OP_SUB,0}, {OP_XOR,0}, {OP_CMP,0}},
        /* c0-c1, d0-d3 - GRP2 */
        {{OP_ROL,0}, {OP_ROR,0}, {OP_RCL,0}, {OP_RCR,0}, {OP_SHL,0}, {OP_SHR,0}, {OP_INVALID,0}, {OP_SAR,0}},
        /* fe - GRP4 */
        {{OP_INC,0}, {OP_DEC,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}},
        /* ff - GRP5 */
        {{OP_INC,0}, {OP_DEC,0}, {OP_CALL,0}, {OP_CALL,1}, {OP_JMP,0}, {OP_JMP,1}, {OP_PUSH,0}, {OP_INVALID,0}},
        /* c6-c7 GRP11 */
        {{OP_MOV,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}},
        /* f6-f7 - GRP3 */
        {{OP_TEST,0}, {OP_INVALID,0}, {OP_NOT,0}, {OP_NEG,0}, {OP_MUL,0}, {OP_IMUL,0}, {OP_DIV,0}, {OP_IDIV,0}},
        /* 0f00 - GRP6 */
        {{OP_SLDT,0}, {OP_STR,0}, {OP_LLDT,0}, {OP_LTR,0}, {OP_VERR,0}, {OP_VERRW,0}, {OP_INVALID,0}, {OP_INVALID,0}},
        /* 0f90 - SETO */
        {{OP_SETO,0}, {OP_SETO,0}, {OP_SETO,0}, {OP_SETO,0}, {OP_SETO,0}, {OP_SETO,0}, {OP_SETO,0}, {OP_SETO,0}},
        /* 0f91 - SETNO */
        {{OP_SETNO,0}, {OP_SETNO,0}, {OP_SETNO,0}, {OP_SETNO,0}, {OP_SETNO,0}, {OP_SETNO,0}, {OP_SETNO,0}, {OP_SETNO,0}},
        /* 0f92 - SETC */
        {{OP_SETC,0}, {OP_SETC,0}, {OP_SETC,0}, {OP_SETC,0}, {OP_SETC,0}, {OP_SETC,0}, {OP_SETC,0}, {OP_SETC,0}},
        /* 0f93 - SETNC */
        {{OP_SETNC,0}, {OP_SETNC,0}, {OP_SETNC,0}, {OP_SETNC,0}, {OP_SETNC,0}, {OP_SETNC,0}, {OP_SETNC,0}, {OP_SETNC,0}},
        /* 0f94 - SETZ */
        {{OP_SETZ,0}, {OP_SETZ,0}, {OP_SETZ,0}, {OP_SETZ,0}, {OP_SETZ,0}, {OP_SETZ,0}, {OP_SETZ,0}, {OP_SETZ,0}},
        /* 0f95 - SETNZ */
        {{OP_SETNO,0}, {OP_SETNO,0}, {OP_SETNO,0}, {OP_SETNO,0}, {OP_SETNO,0}, {OP_SETNO,0}, {OP_SETNO,0}, {OP_SETNO,0}},
        /* 0f96 - SETNB */
        {{OP_SETBE,0}, {OP_SETBE,0}, {OP_SETBE,0}, {OP_SETBE,0}, {OP_SETBE,0}, {OP_SETBE,0}, {OP_SETBE,0}, {OP_SETBE,0}},
        /* 0f97 - SETNA */
        {{OP_SETA,0}, {OP_SETA,0}, {OP_SETA,0}, {OP_SETA,0}, {OP_SETA,0}, {OP_SETA,0}, {OP_SETA,0}, {OP_SETA,0}},
        /* 0f98 - SETS */
        {{OP_SETS,0}, {OP_SETS,0}, {OP_SETS,0}, {OP_SETS,0}, {OP_SETS,0}, {OP_SETS,0}, {OP_SETS,0}, {OP_SETS,0}},
        /* 0f98 - SETNS */
        {{OP_SETNS,0}, {OP_SETNS,0}, {OP_SETNS,0}, {OP_SETNS,0}, {OP_SETNS,0}, {OP_SETNS,0}, {OP_SETNS,0}, {OP_SETNS,0}},
        /* 0f9a - SETP */
        {{OP_SETP,0}, {OP_SETP,0}, {OP_SETP,0}, {OP_SETP,0}, {OP_SETP,0}, {OP_SETP,0}, {OP_SETP,0}, {OP_SETP,0}},
        /* 0f9b - SETNP */
        {{OP_SETNP,0}, {OP_SETNP,0}, {OP_SETNP,0}, {OP_SETNP,0}, {OP_SETNP,0}, {OP_SETNP,0}, {OP_SETNP,0}, {OP_SETNP,0}},
        /* 0f9c - SETL */
        {{OP_SETL,0}, {OP_SETL,0}, {OP_SETL,0}, {OP_SETL,0}, {OP_SETL,0}, {OP_SETL,0}, {OP_SETL,0}, {OP_SETL,0}},
        /* 0f9d - SETGE */
        {{OP_SETGE,0}, {OP_SETGE,0}, {OP_SETGE,0}, {OP_SETGE,0}, {OP_SETGE,0}, {OP_SETGE,0}, {OP_SETGE,0}, {OP_SETGE,0}},
        /* 0f9e - SETLE */
        {{OP_SETLE,0}, {OP_SETLE,0}, {OP_SETLE,0}, {OP_SETLE,0}, {OP_SETLE,0}, {OP_SETLE,0}, {OP_SETLE,0}, {OP_SETLE,0}},
        /* 0f9f - SETG */
        {{OP_SETG,0}, {OP_SETG,0}, {OP_SETG,0}, {OP_SETG,0}, {OP_SETG,0}, {OP_SETG,0}, {OP_SETG,0}, {OP_SETG,0}},
        /* 0fba - GRP8 */
        {{OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_BT,0}, {OP_BTS,0}, {OP_BTR,0}, {OP_BTC,0}},
        /* 0fc7 - GRP9 */
        {{OP_INVALID,0}, {OP_CMPXCHG8B,2}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}},
        /* 0f01 - GRP7 */
        {{OP_SGDT,3}, {OP_SIDT,3}, {OP_LGDT,3}, {OP_LIDT,3}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVALID,0}, {OP_INVLPG,0}},
    };

#define PUSHOP(a,b,c,d,e,f) {b,c,d,e,f}
#define OP_UNSUP OP_INVALID
    static const struct OPCODES x86ops[2][256] = {{
            PUSHOP(0x00, ADDR_MRM_GEN_EG, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_ADD),
            PUSHOP(0x01, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_ADD),
            PUSHOP(0x02, ADDR_MRM_GEN_GE, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_ADD),
            PUSHOP(0x03, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_ADD),
            PUSHOP(0x04, ADDR_REG_EAX, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, OP_ADD),
            PUSHOP(0x05, ADDR_REG_EAX, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_ADD),
            PUSHOP(0x06, ADDR_REG_ES, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0x07, ADDR_REG_ES, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_POP),
            PUSHOP(0x08, ADDR_MRM_GEN_EG, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_OR),
            PUSHOP(0x09, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_OR),
            PUSHOP(0x0a, ADDR_MRM_GEN_GE, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_OR),
            PUSHOP(0x0b, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_OR),
            PUSHOP(0x0c, ADDR_REG_EAX, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, OP_OR),
            PUSHOP(0x0d, ADDR_REG_EAX, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_OR),
            PUSHOP(0x0e, ADDR_REG_CS, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0x0f, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_2BYTE),

            PUSHOP(0x10, ADDR_MRM_GEN_EG, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_ADC),
            PUSHOP(0x11, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_ADC),
            PUSHOP(0x12, ADDR_MRM_GEN_GE, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_ADC),
            PUSHOP(0x13, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_ADC),
            PUSHOP(0x14, ADDR_REG_EAX, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, OP_ADC),
            PUSHOP(0x15, ADDR_REG_EAX, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_ADC),
            PUSHOP(0x16, ADDR_REG_SS, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0x17, ADDR_REG_SS, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_POP),
            PUSHOP(0x18, ADDR_MRM_GEN_EG, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_SBB),
            PUSHOP(0x19, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_SBB),
            PUSHOP(0x1a, ADDR_MRM_GEN_GE, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_SBB),
            PUSHOP(0x1b, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_SBB),
            PUSHOP(0x1c, ADDR_REG_EAX, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, OP_SBB),
            PUSHOP(0x1d, ADDR_REG_EAX, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_SBB),
            PUSHOP(0x1e, ADDR_REG_DS, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0x1f, ADDR_REG_DS, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_POP),

            PUSHOP(0x20, ADDR_MRM_GEN_EG, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_AND),
            PUSHOP(0x21, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_AND),
            PUSHOP(0x22, ADDR_MRM_GEN_GE, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_AND),
            PUSHOP(0x23, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_AND),
            PUSHOP(0x24, ADDR_REG_EAX, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, OP_AND),
            PUSHOP(0x25, ADDR_REG_EAX, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_AND),
            PUSHOP(0x26, ADDR_REG_ES, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_PREFIX_SEGMENT),
            PUSHOP(0x27, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_DAA),
            PUSHOP(0x28, ADDR_MRM_GEN_EG, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_SUB),
            PUSHOP(0x29, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_SUB),
            PUSHOP(0x2a, ADDR_MRM_GEN_GE, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_SUB),
            PUSHOP(0x2b, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_SUB),
            PUSHOP(0x2c, ADDR_REG_EAX, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, OP_SUB),
            PUSHOP(0x2d, ADDR_REG_EAX, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_SUB),
            PUSHOP(0x2e, ADDR_REG_CS, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_PREFIX_SEGMENT),
            PUSHOP(0x2f, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_DAS),

            PUSHOP(0x30, ADDR_MRM_GEN_EG, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_XOR),
            PUSHOP(0x31, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_XOR),
            PUSHOP(0x32, ADDR_MRM_GEN_GE, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_XOR),
            PUSHOP(0x33, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_XOR),
            PUSHOP(0x34, ADDR_REG_EAX, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, OP_XOR),
            PUSHOP(0x35, ADDR_REG_EAX, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_XOR),
            PUSHOP(0x36, ADDR_REG_SS, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_PREFIX_SEGMENT),
            PUSHOP(0x37, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_AAA),
            PUSHOP(0x38, ADDR_MRM_GEN_EG, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_CMP),
            PUSHOP(0x39, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMP),
            PUSHOP(0x3a, ADDR_MRM_GEN_GE, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_CMP),
            PUSHOP(0x3b, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMP),
            PUSHOP(0x3c, ADDR_REG_EAX, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, OP_CMP),
            PUSHOP(0x3d, ADDR_REG_EAX, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_CMP),
            PUSHOP(0x3e, ADDR_REG_DS, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_PREFIX_SEGMENT),
            PUSHOP(0x3f, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_AAS),

            PUSHOP(0x40, ADDR_REG_EAX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_INC),
            PUSHOP(0x41, ADDR_REG_ECX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_INC),
            PUSHOP(0x42, ADDR_REG_EDX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_INC),
            PUSHOP(0x43, ADDR_REG_EBX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_INC),
            PUSHOP(0x44, ADDR_REG_ESP, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_INC),
            PUSHOP(0x45, ADDR_REG_EBP, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_INC),
            PUSHOP(0x46, ADDR_REG_ESI, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_INC),
            PUSHOP(0x47, ADDR_REG_EDI, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_INC),
            PUSHOP(0x48, ADDR_REG_EAX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_DEC),
            PUSHOP(0x49, ADDR_REG_ECX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_DEC),
            PUSHOP(0x4a, ADDR_REG_EDX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_DEC),
            PUSHOP(0x4b, ADDR_REG_EBX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_DEC),
            PUSHOP(0x4c, ADDR_REG_ESP, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_DEC),
            PUSHOP(0x4d, ADDR_REG_EBP, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_DEC),
            PUSHOP(0x4e, ADDR_REG_ESI, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_DEC),
            PUSHOP(0x4f, ADDR_REG_EDI, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_DEC),

            PUSHOP(0x50, ADDR_REG_EAX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0x51, ADDR_REG_ECX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0x52, ADDR_REG_EDX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0x53, ADDR_REG_EBX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0x54, ADDR_REG_ESP, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0x55, ADDR_REG_EBP, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0x56, ADDR_REG_ESI, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0x57, ADDR_REG_EDI, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0x58, ADDR_REG_EAX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_POP),
            PUSHOP(0x59, ADDR_REG_ECX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_POP),
            PUSHOP(0x5a, ADDR_REG_EDX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_POP),
            PUSHOP(0x5b, ADDR_REG_EBX, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_POP),
            PUSHOP(0x5c, ADDR_REG_ESP, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_POP),
            PUSHOP(0x5d, ADDR_REG_EBP, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_POP),
            PUSHOP(0x5e, ADDR_REG_ESI, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_POP),
            PUSHOP(0x5f, ADDR_REG_EDI, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_POP),

            PUSHOP(0x60, ADDR_NOADDR, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSHAD),
            PUSHOP(0x61, ADDR_NOADDR, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_POPAD),
            PUSHOP(0x62, ADDR_MRM_GEN_GM, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_BOUND),
            PUSHOP(0x63, ADDR_MRM_GEN_EG, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_ARPL),
            PUSHOP(0x64, ADDR_REG_FS, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_PREFIX_SEGMENT),
            PUSHOP(0x65, ADDR_REG_GS, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_PREFIX_SEGMENT),
            PUSHOP(0x66, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_PREFIX_OPSIZE),
            PUSHOP(0x67, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_PREFIX_ADDRSIZE),
            PUSHOP(0x68, ADDR_IMMED, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0x69, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_IMUL),
            PUSHOP(0x6a, ADDR_IMMED, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0x6b, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_IMMED, SIZE_BYTE, OP_IMUL),
            PUSHOP(0x6c, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INSB),
            PUSHOP(0x6d, ADDR_NOADDR, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_INSD),
            PUSHOP(0x6e, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_OUTSB),
            PUSHOP(0x6f, ADDR_NOADDR, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_OUTSD),

            PUSHOP(0x70, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JO),
            PUSHOP(0x71, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JNO),
            PUSHOP(0x72, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JC),
            PUSHOP(0x73, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JNC),
            PUSHOP(0x74, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JZ),
            PUSHOP(0x75, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JNZ),
            PUSHOP(0x76, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JBE),
            PUSHOP(0x77, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JA),
            PUSHOP(0x78, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JS),
            PUSHOP(0x79, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JNS),
            PUSHOP(0x7a, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JP),
            PUSHOP(0x7b, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JNP),
            PUSHOP(0x7c, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JL),
            PUSHOP(0x7d, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JGE),
            PUSHOP(0x7e, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JLE),
            PUSHOP(0x7f, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JG),

            PUSHOP(0x80, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, 1),
            PUSHOP(0x81, ADDR_MRM_EXTRA_1A, SIZE_WD, ADDR_IMMED, SIZE_WD, 1),
            PUSHOP(0x82, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, 1),
            PUSHOP(0x83, ADDR_MRM_EXTRA_1A, SIZE_WD, ADDR_IMMED, SIZE_BYTE, 1),
            PUSHOP(0x84, ADDR_MRM_GEN_EG, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_TEST),
            PUSHOP(0x85, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_TEST),
            PUSHOP(0x86, ADDR_MRM_GEN_EG, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_XCHG),
            PUSHOP(0x87, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_XCHG),
            PUSHOP(0x88, ADDR_MRM_GEN_EG, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_MOV),
            PUSHOP(0x89, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_MOV),
            PUSHOP(0x8a, ADDR_MRM_GEN_GE, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_MOV),
            PUSHOP(0x8b, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_MOV),
            PUSHOP(0x8c, ADDR_MRM_GEN_ES, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_MOV),
            PUSHOP(0x8d, ADDR_MRM_GEN_GM, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_LEA),
            PUSHOP(0x8e, ADDR_MRM_GEN_SE, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_MOV),
            PUSHOP(0x8f, ADDR_MRM_EXTRA_1A, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, 0),

            PUSHOP(0x90, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_NOP),
            PUSHOP(0x91, ADDR_REG_ECX, SIZE_WD, ADDR_REG_EAX, SIZE_WD, OP_XCHG),
            PUSHOP(0x92, ADDR_REG_EDX, SIZE_WD, ADDR_REG_EAX, SIZE_WD, OP_XCHG),
            PUSHOP(0x93, ADDR_REG_EBX, SIZE_WD, ADDR_REG_EAX, SIZE_WD, OP_XCHG),
            PUSHOP(0x94, ADDR_REG_ESP, SIZE_WD, ADDR_REG_EAX, SIZE_WD, OP_XCHG),
            PUSHOP(0x95, ADDR_REG_EBP, SIZE_WD, ADDR_REG_EAX, SIZE_WD, OP_XCHG),
            PUSHOP(0x96, ADDR_REG_ESI, SIZE_WD, ADDR_REG_EAX, SIZE_WD, OP_XCHG),
            PUSHOP(0x97, ADDR_REG_EDI, SIZE_WD, ADDR_REG_EAX, SIZE_WD, OP_XCHG),
            PUSHOP(0x98, ADDR_NOADDR, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CWDE),
            PUSHOP(0x99, ADDR_NOADDR, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CDQ),
            PUSHOP(0x9a, ADDR_IMMED, SIZE_DF, ADDR_NOADDR, SIZE_NOSIZE, OP_CALL),
            PUSHOP(0x9b, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_FWAIT),
            PUSHOP(0x9c, ADDR_NOADDR, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSHFD),
            PUSHOP(0x9d, ADDR_NOADDR, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_POPFD),
            PUSHOP(0x9e, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_SAHF),
            PUSHOP(0x9f, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_LAHF),

            PUSHOP(0xa0, ADDR_REG_EAX, SIZE_BYTE, ADDR_OFFSET, SIZE_BYTE, OP_MOV),
            PUSHOP(0xa1, ADDR_REG_EAX, SIZE_WD, ADDR_OFFSET, SIZE_WD, OP_MOV),
            PUSHOP(0xa2, ADDR_OFFSET, SIZE_BYTE, ADDR_REG_EAX, SIZE_BYTE, OP_MOV),
            PUSHOP(0xa3, ADDR_OFFSET, SIZE_WD, ADDR_REG_EAX, SIZE_WD, OP_MOV),
            PUSHOP(0xa4, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_MOVSB),
            PUSHOP(0xa5, ADDR_NOADDR, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_MOVSD),
            PUSHOP(0xa6, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_CMPSB),
            PUSHOP(0xa7, ADDR_NOADDR, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMPSD),
            PUSHOP(0xa8, ADDR_REG_EAX, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, OP_TEST),
            PUSHOP(0xa9, ADDR_REG_EAX, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_TEST),
            PUSHOP(0xaa, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_STOSB),
            PUSHOP(0xab, ADDR_NOADDR, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_STOSD),
            PUSHOP(0xac, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_LODSB),
            PUSHOP(0xad, ADDR_NOADDR, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_LODSD),
            PUSHOP(0xae, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_SCASB),
            PUSHOP(0xaf, ADDR_NOADDR, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_SCASD),

            PUSHOP(0xb0, ADDR_REG_EAX, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, OP_MOV),
            PUSHOP(0xb1, ADDR_REG_ECX, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, OP_MOV),
            PUSHOP(0xb2, ADDR_REG_EDX, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, OP_MOV),
            PUSHOP(0xb3, ADDR_REG_EBX, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, OP_MOV),
            PUSHOP(0xb4, ADDR_REG_EAX, SIZE_BYTEH, ADDR_IMMED, SIZE_BYTE, OP_MOV),
            PUSHOP(0xb5, ADDR_REG_ECX, SIZE_BYTEH, ADDR_IMMED, SIZE_BYTE, OP_MOV),
            PUSHOP(0xb6, ADDR_REG_EDX, SIZE_BYTEH, ADDR_IMMED, SIZE_BYTE, OP_MOV),
            PUSHOP(0xb7, ADDR_REG_EBX, SIZE_BYTEH, ADDR_IMMED, SIZE_BYTE, OP_MOV),
            PUSHOP(0xb8, ADDR_REG_EAX, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_MOV),
            PUSHOP(0xb9, ADDR_REG_ECX, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_MOV),
            PUSHOP(0xba, ADDR_REG_EDX, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_MOV),
            PUSHOP(0xbb, ADDR_REG_EBX, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_MOV),
            PUSHOP(0xbc, ADDR_REG_ESP, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_MOV),
            PUSHOP(0xbd, ADDR_REG_EBP, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_MOV),
            PUSHOP(0xbe, ADDR_REG_ESI, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_MOV),
            PUSHOP(0xbf, ADDR_REG_EDI, SIZE_WD, ADDR_IMMED, SIZE_WD, OP_MOV),

            PUSHOP(0xc0, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, 2),
            PUSHOP(0xc1, ADDR_MRM_EXTRA_1A, SIZE_WD, ADDR_IMMED, SIZE_BYTE, 2),
            PUSHOP(0xc2, ADDR_IMMED, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_RETN),
            PUSHOP(0xc3, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_RETN),
            PUSHOP(0xc4, ADDR_MRM_GEN_GM, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_LES), /* FIXME: mem size is F/D */
            PUSHOP(0xc5, ADDR_MRM_GEN_GM, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_LDS), /* FIXME: mem size is F/D */
            PUSHOP(0xc6, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, 5),
            PUSHOP(0xc7, ADDR_MRM_EXTRA_1A, SIZE_WD, ADDR_IMMED, SIZE_WD, 5),
            PUSHOP(0xc8, ADDR_IMMED, SIZE_WORD, ADDR_IMMED, SIZE_BYTE, OP_ENTER),
            PUSHOP(0xc9, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_LEAVE),
            PUSHOP(0xca, ADDR_IMMED, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_RETF),
            PUSHOP(0xcb, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_RETF),
            PUSHOP(0xcc, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INT3),
            PUSHOP(0xcd, ADDR_IMMED, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_INT),
            PUSHOP(0xce, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INTO),
            PUSHOP(0xcf, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_IRET),

            PUSHOP(0xd0, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_IMPLICIT, 1, 2),
            PUSHOP(0xd1, ADDR_MRM_EXTRA_1A, SIZE_WD, ADDR_IMPLICIT, 1, 2),
            PUSHOP(0xd2, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_REG_ECX, SIZE_BYTE, 2),
            PUSHOP(0xd3, ADDR_MRM_EXTRA_1A, SIZE_WD, ADDR_REG_ECX, SIZE_BYTE, 2),
            PUSHOP(0xd4, ADDR_IMMED, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_AAM),
            PUSHOP(0xd5, ADDR_IMMED, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_AAD),
            PUSHOP(0xd6, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0xd7, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_XLAT),
            PUSHOP(0xd8, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_FPU),
            PUSHOP(0xd9, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_FPU),
            PUSHOP(0xda, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_FPU),
            PUSHOP(0xdb, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_FPU),
            PUSHOP(0xdc, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_FPU),
            PUSHOP(0xdd, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_FPU),
            PUSHOP(0xde, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_FPU),
            PUSHOP(0xdf, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_FPU),

            PUSHOP(0xe0, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_LOOPNE),
            PUSHOP(0xe1, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_LOOPE),
            PUSHOP(0xe2, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_LOOP),
            PUSHOP(0xe3, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JECXZ),
            PUSHOP(0xe4, ADDR_REG_EAX, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, OP_IN),
            PUSHOP(0xe5, ADDR_REG_EAX, SIZE_WD, ADDR_IMMED, SIZE_BYTE, OP_IN),
            PUSHOP(0xe6, ADDR_IMMED, SIZE_BYTE, ADDR_REG_EAX, SIZE_BYTE, OP_OUT),
            PUSHOP(0xe7, ADDR_IMMED, SIZE_BYTE, ADDR_REG_EAX, SIZE_WD, OP_OUT),
            PUSHOP(0xe8, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CALL),
            PUSHOP(0xe9, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JMP),
            PUSHOP(0xea, ADDR_IMMED, SIZE_DF, ADDR_NOADDR, SIZE_NOSIZE, OP_JMP),
            PUSHOP(0xeb, ADDR_RELJ, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_JMP),
            PUSHOP(0xec, ADDR_REG_EAX, SIZE_BYTE, ADDR_REG_EDX, SIZE_WORD, OP_IN),
            PUSHOP(0xed, ADDR_REG_EAX, SIZE_WD, ADDR_REG_EDX, SIZE_WORD, OP_IN),
            PUSHOP(0xee, ADDR_REG_EDX, SIZE_WORD, ADDR_REG_EAX, SIZE_BYTE, OP_OUT),
            PUSHOP(0xef, ADDR_REG_EDX, SIZE_WORD, ADDR_REG_EAX, SIZE_WD, OP_OUT),

            PUSHOP(0xf0, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_PREFIX_LOCK),
            PUSHOP(0xf1, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0xf2, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_PREFIX_REPNE),
            PUSHOP(0xf3, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_PREFIX_REPE),
            PUSHOP(0xf4, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_HLT),
            PUSHOP(0xf5, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_CMC),
            PUSHOP(0xf6, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_IMMED, SIZE_BYTE, 6),
            PUSHOP(0xf7, ADDR_MRM_EXTRA_1A, SIZE_WD, ADDR_IMMED, SIZE_WD, 6),
            PUSHOP(0xf8, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_CLC),
            PUSHOP(0xf9, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_STC),
            PUSHOP(0xfa, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_CLI),
            PUSHOP(0xfb, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_STI),
            PUSHOP(0xfc, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_CLD),
            PUSHOP(0xfd, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_STD),
            PUSHOP(0xfe, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 3),
            PUSHOP(0xff, ADDR_MRM_EXTRA_1A, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, 4),
        },{ /* 222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222 */
            PUSHOP(0x00, ADDR_MRM_EXTRA_1A, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, 7),
            PUSHOP(0x01, ADDR_MRM_EXTRA_1A_M, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 26), /* FIXME: SMSW/LMSW Mw/R[vw] (0f0120,0f0130,660f0120,660f0130,0f01e0,0f01f0,660f01e0,660f01f0) not handled */
            PUSHOP(0x02, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_LAR), /* FIXME: ssize is always W */
            PUSHOP(0x03, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_LSL), /* FIXME: ssize is always W */
            PUSHOP(0x04, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x05, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_SYSCALL),
            PUSHOP(0x06, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_CLTS),
            PUSHOP(0x07, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_SYSRET),
            PUSHOP(0x08, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVD),
            PUSHOP(0x09, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_WBINVD),
            PUSHOP(0x0a, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x0b, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UD2),
            PUSHOP(0x0c, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x0d, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x0e, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x0f, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),

            PUSHOP(0x10, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x11, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x12, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x13, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x14, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x15, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x16, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x17, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x18, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x19, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x1a, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x1b, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x1c, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x1d, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x1e, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x1f, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),

            PUSHOP(0x20, ADDR_MRM_GEN_RC, SIZE_DWORD, ADDR_NOADDR, SIZE_NOSIZE, OP_MOV),
            PUSHOP(0x21, ADDR_MRM_GEN_RD, SIZE_DWORD, ADDR_NOADDR, SIZE_NOSIZE, OP_MOV),
            PUSHOP(0x22, ADDR_MRM_GEN_CR, SIZE_DWORD, ADDR_NOADDR, SIZE_NOSIZE, OP_MOV),
            PUSHOP(0x23, ADDR_MRM_GEN_DR, SIZE_DWORD, ADDR_NOADDR, SIZE_NOSIZE, OP_MOV),
            PUSHOP(0x24, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x25, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x26, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x27, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x28, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x29, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x2a, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x2b, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x2c, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x2d, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x2e, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x2f, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),

            PUSHOP(0x30, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_WRMSR),
            PUSHOP(0x31, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_RDTSC),
            PUSHOP(0x32, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_RDMSR),
            PUSHOP(0x33, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_RDPMC),
            PUSHOP(0x34, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_SYSENTER),
            PUSHOP(0x35, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_SYSEXIT),
            PUSHOP(0x36, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x37, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x38, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP), /* 3byte escape */
            PUSHOP(0x39, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x3a, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP), /* 3byte escape */
            PUSHOP(0x3b, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x3c, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x3d, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x3e, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x3f, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),

            PUSHOP(0x40, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVO),
            PUSHOP(0x41, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVNO),
            PUSHOP(0x42, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVC),
            PUSHOP(0x43, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVNC),
            PUSHOP(0x44, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVZ),
            PUSHOP(0x45, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVNZ),
            PUSHOP(0x46, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVBE),
            PUSHOP(0x47, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVA),
            PUSHOP(0x48, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVS),
            PUSHOP(0x49, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVNS),
            PUSHOP(0x4a, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVP),
            PUSHOP(0x4b, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVNP),
            PUSHOP(0x4c, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVL),
            PUSHOP(0x4d, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVGE),
            PUSHOP(0x4e, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVLE),
            PUSHOP(0x4f, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMOVG),

            PUSHOP(0x50, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x51, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x52, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x53, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x54, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x55, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x56, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x57, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x58, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x59, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x5a, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x5b, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x5c, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x5d, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x5e, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x5f, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),

            PUSHOP(0x60, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x61, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x62, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x63, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x64, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x65, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x66, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x67, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x68, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x69, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x6a, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x6b, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x6c, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x6d, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x6e, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x6f, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),

            PUSHOP(0x70, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x71, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x72, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x73, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x74, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x75, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x76, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x77, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x78, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x79, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x7a, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x7b, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0x7c, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x7d, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x7e, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0x7f, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),

            PUSHOP(0x80, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JO),
            PUSHOP(0x81, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JNO),
            PUSHOP(0x82, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JC),
            PUSHOP(0x83, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JNC),
            PUSHOP(0x84, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JZ),
            PUSHOP(0x85, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JNZ),
            PUSHOP(0x86, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JBE),
            PUSHOP(0x87, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JA),
            PUSHOP(0x88, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JS),
            PUSHOP(0x89, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JNS),
            PUSHOP(0x8a, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JP),
            PUSHOP(0x8b, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JNP),
            PUSHOP(0x8c, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JL),
            PUSHOP(0x8d, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JGE),
            PUSHOP(0x8e, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JLE),
            PUSHOP(0x8f, ADDR_RELJ, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_JG),

            PUSHOP(0x90, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 8),
            PUSHOP(0x91, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 9),
            PUSHOP(0x92, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 10),
            PUSHOP(0x93, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 11),
            PUSHOP(0x94, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 12),
            PUSHOP(0x95, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 13),
            PUSHOP(0x96, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 14),
            PUSHOP(0x97, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 15),
            PUSHOP(0x98, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 16),
            PUSHOP(0x99, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 17),
            PUSHOP(0x9a, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 18),
            PUSHOP(0x9b, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 19),
            PUSHOP(0x9c, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 20),
            PUSHOP(0x9d, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 21),
            PUSHOP(0x9e, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 22),
            PUSHOP(0x9f, ADDR_MRM_EXTRA_1A, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, 23),

            PUSHOP(0xa0, ADDR_REG_FS, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0xa1, ADDR_REG_FS, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_POP),
            PUSHOP(0xa2, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_CPUID),
            PUSHOP(0xa3, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_BT),
            PUSHOP(0xa4, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_IMMED, SIZE_BYTE, OP_SHLD),
            PUSHOP(0xa5, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_REG_ECX, SIZE_BYTE, OP_SHLD),
            PUSHOP(0xa6, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0xa7, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0xa8, ADDR_REG_GS, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_PUSH),
            PUSHOP(0xa9, ADDR_REG_GS, SIZE_WORD, ADDR_NOADDR, SIZE_NOSIZE, OP_POP),
            PUSHOP(0xaa, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_RSM),
            PUSHOP(0xab, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_BTS),
            PUSHOP(0xac, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_IMMED, SIZE_BYTE, OP_SHRD),
            PUSHOP(0xad, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_REG_ECX, SIZE_BYTE, OP_SHRD),
            PUSHOP(0xae, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xaf, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_IMUL),

            PUSHOP(0xb0, ADDR_MRM_GEN_EG, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_CMPXCHG),
            PUSHOP(0xb1, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_CMPXCHG),
            PUSHOP(0xb2, ADDR_MRM_GEN_GM, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_LSS), /* FIXME: mem size is F/D */
            PUSHOP(0xb3, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_BTR),
            PUSHOP(0xb4, ADDR_MRM_GEN_GM, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_LFS), /* FIXME: mem size is F/D */
            PUSHOP(0xb5, ADDR_MRM_GEN_GM, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_LGS), /* FIXME: mem size is F/D */
            PUSHOP(0xb6, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_MOVZX),
            PUSHOP(0xb7, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_MOVZX),
            PUSHOP(0xb8, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),
            PUSHOP(0xb9, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xba, ADDR_MRM_EXTRA_1A, SIZE_WD, ADDR_IMMED, SIZE_BYTE, 24),
            PUSHOP(0xbb, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_BTC),
            PUSHOP(0xbc, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_BSF),
            PUSHOP(0xbd, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_BSR),
            PUSHOP(0xbe, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_MOVSX),
            PUSHOP(0xbf, ADDR_MRM_GEN_GE, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_MOVSX),

            PUSHOP(0xc0, ADDR_MRM_GEN_EG, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_XADD),
            PUSHOP(0xc1, ADDR_MRM_GEN_EG, SIZE_WD, ADDR_NOADDR, SIZE_NOSIZE, OP_XADD),
            PUSHOP(0xc2, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xc3, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xc4, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xc5, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xc6, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xc7, ADDR_MRM_EXTRA_1A_M, SIZE_DWORD, ADDR_NOADDR, SIZE_NOSIZE, 25),
            PUSHOP(0xc8, ADDR_REG_EAX, SIZE_DWORD, ADDR_NOADDR, SIZE_NOSIZE, OP_BSWAP),
            PUSHOP(0xc9, ADDR_REG_ECX, SIZE_DWORD, ADDR_NOADDR, SIZE_NOSIZE, OP_BSWAP),
            PUSHOP(0xca, ADDR_REG_EDX, SIZE_DWORD, ADDR_NOADDR, SIZE_NOSIZE, OP_BSWAP),
            PUSHOP(0xcb, ADDR_REG_EBX, SIZE_DWORD, ADDR_NOADDR, SIZE_NOSIZE, OP_BSWAP),
            PUSHOP(0xcc, ADDR_REG_ESP, SIZE_DWORD, ADDR_NOADDR, SIZE_NOSIZE, OP_BSWAP),
            PUSHOP(0xcd, ADDR_REG_EBP, SIZE_DWORD, ADDR_NOADDR, SIZE_NOSIZE, OP_BSWAP),
            PUSHOP(0xce, ADDR_REG_ESI, SIZE_DWORD, ADDR_NOADDR, SIZE_NOSIZE, OP_BSWAP),
            PUSHOP(0xcf, ADDR_REG_EDI, SIZE_DWORD, ADDR_NOADDR, SIZE_NOSIZE, OP_BSWAP),

            PUSHOP(0xd0, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xd1, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xd2, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xd3, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xd4, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xd5, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xd6, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xd7, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xd8, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xd9, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xda, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xdb, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xdc, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xdd, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xde, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xdf, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),

            PUSHOP(0xe0, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xe1, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xe2, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xe3, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xe4, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xe5, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xe6, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xe7, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xe8, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xe9, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xea, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xeb, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xec, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xed, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xee, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xef, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),

            PUSHOP(0xf0, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xf1, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xf2, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xf3, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xf4, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xf5, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xf6, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xf7, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xf8, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xf9, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xfa, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xfb, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xfc, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xfd, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xfe, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_UNSUP),
            PUSHOP(0xff, ADDR_NOADDR, SIZE_NOSIZE, ADDR_NOADDR, SIZE_NOSIZE, OP_INVALID),

        }
    };


    static const char *dis_size[] = {"byte", "word", "dword", "fword", "qword", "tword", "acab"};

    static const char *x86regs[] = {
        "eax","ecx","edx","ebx","esp","ebp","esi","edi",
        "ax","cx","dx","bx","sp","bp","si","di",
        "ah","ch","dh","bh","al","cl","dl","bl",
        "es","cs","ss","ds","fs","gs",
        "cr0", "cr1 (rsvd)", "cr2", "cr3", "cr4", "cr5 (rsvd)", "cr6 (rsvd)", "cr7 (rsvd)",
        "dr0", "dr1", "dr2", "dr3", "dr4 (rsvd)", "dr5 (rsvd)", "dr6", "dr7",
        "st(0)", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)",
        ""
    };


#define INVALIDATE {s->table_op=OP_INVALID; s->state=STATE_ERROR; goto disasm_failed;}
#define GETBYTE(X) if(len--) {X=*command; command++;} else INVALIDATE;
#define GETSIZE(X) (x86ops[table][s->table_op].X!=SIZE_WD?x86ops[table][s->table_op].X:((s->opsize)?SIZE_WORD:SIZE_DWORD))




    class disasmx86_controller
    {

        public:

            inline static void spam_x86(struct DISASMED *s, char *hr);

            inline static const uint8_t *disasm_x86(const uint8_t *command,
                    unsigned int len,
                    struct DISASMED *s);

            const uint8_t *cli_disasm_one(const uint8_t *buff,
                    unsigned int len,
                    struct DISASM_RESULT *w,
                    int spam)

            int disasmbuf(const uint8_t *buff,
                    unsigned int len,
                    int fd);

        pirvate:

    };

}



#endif /* DISASM86_CONTROLLER_HPP */
