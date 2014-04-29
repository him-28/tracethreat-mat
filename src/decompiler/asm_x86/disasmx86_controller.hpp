#ifndef  DISASMS_DISASMX86_CONTROLLER_HPP
#define  DISASMS_DISASMX86_CONTROLLER_HPP

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
#include <stdint.h>

#include "decompiler/asm_x86/disasmpriv.hpp"

namespace disasms
{
    //static const struct x87_st;

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


enum arg{ X87_NONE, X87_ONE, X87_S, X87_R };

struct{
  enum X86OPS op;
	enum arg   args;
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

enum addszs{ ADDSZ_ZERO = 0, ADDSZ_ONE = 1, ADDSZ_TWO = 2, ADDSZ_THREE =3 };

static const struct {
  enum X86OPS op;
  enum addszs  addsz;
  } extra_1a[][8] = {
  /* 8f - pop */
  {{OP_POP, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }},
  /* 8 ADDSZ_ZERO -8 ADDSZ_THREE - GRP ADDSZ_ONE */
  {{OP_ADD, ADDSZ_ZERO }, {OP_OR, ADDSZ_ZERO }, {OP_ADC, ADDSZ_ZERO }, {OP_SBB, ADDSZ_ZERO }, {OP_AND, ADDSZ_ZERO }, {OP_SUB, ADDSZ_ZERO }, {OP_XOR, ADDSZ_ZERO }, {OP_CMP, ADDSZ_ZERO }},
  /* c ADDSZ_ZERO -c ADDSZ_ONE, d ADDSZ_ZERO -d ADDSZ_THREE - GRP ADDSZ_TWO */
  {{OP_ROL, ADDSZ_ZERO }, {OP_ROR, ADDSZ_ZERO }, {OP_RCL, ADDSZ_ZERO }, {OP_RCR, ADDSZ_ZERO }, {OP_SHL, ADDSZ_ZERO }, {OP_SHR, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_SAR, ADDSZ_ZERO }},
  /* fe - GRP4 */
  {{OP_INC, ADDSZ_ZERO }, {OP_DEC, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }},
  /* ff - GRP5 */
  {{OP_INC, ADDSZ_ZERO }, {OP_DEC, ADDSZ_ZERO }, {OP_CALL, ADDSZ_ZERO }, {OP_CALL, ADDSZ_ONE}, {OP_JMP, ADDSZ_ZERO }, {OP_JMP, ADDSZ_ONE}, {OP_PUSH, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }},
  /* c6-c7 GRP ADDSZ_ONE ADDSZ_ONE */
  {{OP_MOV, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }},
  /* f6-f7 - GRP ADDSZ_THREE */
  {{OP_TEST, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_NOT, ADDSZ_ZERO }, {OP_NEG, ADDSZ_ZERO }, {OP_MUL, ADDSZ_ZERO }, {OP_IMUL, ADDSZ_ZERO }, {OP_DIV, ADDSZ_ZERO }, {OP_IDIV, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f ADDSZ_ZERO  ADDSZ_ZERO  - GRP6 */
  {{OP_SLDT, ADDSZ_ZERO }, {OP_STR, ADDSZ_ZERO }, {OP_LLDT, ADDSZ_ZERO }, {OP_LTR, ADDSZ_ZERO }, {OP_VERR, ADDSZ_ZERO }, {OP_VERRW, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f9 ADDSZ_ZERO  - SETO */
  {{OP_SETO, ADDSZ_ZERO }, {OP_SETO, ADDSZ_ZERO }, {OP_SETO, ADDSZ_ZERO }, {OP_SETO, ADDSZ_ZERO }, {OP_SETO, ADDSZ_ZERO }, {OP_SETO, ADDSZ_ZERO }, {OP_SETO, ADDSZ_ZERO }, {OP_SETO, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f9 ADDSZ_ONE - SETNO */
  {{OP_SETNO, ADDSZ_ZERO }, {OP_SETNO, ADDSZ_ZERO }, {OP_SETNO, ADDSZ_ZERO }, {OP_SETNO, ADDSZ_ZERO }, {OP_SETNO, ADDSZ_ZERO }, {OP_SETNO, ADDSZ_ZERO }, {OP_SETNO, ADDSZ_ZERO }, {OP_SETNO, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f9 ADDSZ_TWO - SETC */
  {{OP_SETC, ADDSZ_ZERO }, {OP_SETC, ADDSZ_ZERO }, {OP_SETC, ADDSZ_ZERO }, {OP_SETC, ADDSZ_ZERO }, {OP_SETC, ADDSZ_ZERO }, {OP_SETC, ADDSZ_ZERO }, {OP_SETC, ADDSZ_ZERO }, {OP_SETC, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f9 ADDSZ_THREE - SETNC */
  {{OP_SETNC, ADDSZ_ZERO }, {OP_SETNC, ADDSZ_ZERO }, {OP_SETNC, ADDSZ_ZERO }, {OP_SETNC, ADDSZ_ZERO }, {OP_SETNC, ADDSZ_ZERO }, {OP_SETNC, ADDSZ_ZERO }, {OP_SETNC, ADDSZ_ZERO }, {OP_SETNC, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f94 - SETZ */
  {{OP_SETZ, ADDSZ_ZERO }, {OP_SETZ, ADDSZ_ZERO }, {OP_SETZ, ADDSZ_ZERO }, {OP_SETZ, ADDSZ_ZERO }, {OP_SETZ, ADDSZ_ZERO }, {OP_SETZ, ADDSZ_ZERO }, {OP_SETZ, ADDSZ_ZERO }, {OP_SETZ, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f95 - SETNZ */
  {{OP_SETNO, ADDSZ_ZERO }, {OP_SETNO, ADDSZ_ZERO }, {OP_SETNO, ADDSZ_ZERO }, {OP_SETNO, ADDSZ_ZERO }, {OP_SETNO, ADDSZ_ZERO }, {OP_SETNO, ADDSZ_ZERO }, {OP_SETNO, ADDSZ_ZERO }, {OP_SETNO, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f96 - SETNB */
  {{OP_SETBE, ADDSZ_ZERO }, {OP_SETBE, ADDSZ_ZERO }, {OP_SETBE, ADDSZ_ZERO }, {OP_SETBE, ADDSZ_ZERO }, {OP_SETBE, ADDSZ_ZERO }, {OP_SETBE, ADDSZ_ZERO }, {OP_SETBE, ADDSZ_ZERO }, {OP_SETBE, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f97 - SETNA */
  {{OP_SETA, ADDSZ_ZERO }, {OP_SETA, ADDSZ_ZERO }, {OP_SETA, ADDSZ_ZERO }, {OP_SETA, ADDSZ_ZERO }, {OP_SETA, ADDSZ_ZERO }, {OP_SETA, ADDSZ_ZERO }, {OP_SETA, ADDSZ_ZERO }, {OP_SETA, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f98 - SETS */
  {{OP_SETS, ADDSZ_ZERO }, {OP_SETS, ADDSZ_ZERO }, {OP_SETS, ADDSZ_ZERO }, {OP_SETS, ADDSZ_ZERO }, {OP_SETS, ADDSZ_ZERO }, {OP_SETS, ADDSZ_ZERO }, {OP_SETS, ADDSZ_ZERO }, {OP_SETS, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f98 - SETNS */
  {{OP_SETNS, ADDSZ_ZERO }, {OP_SETNS, ADDSZ_ZERO }, {OP_SETNS, ADDSZ_ZERO }, {OP_SETNS, ADDSZ_ZERO }, {OP_SETNS, ADDSZ_ZERO }, {OP_SETNS, ADDSZ_ZERO }, {OP_SETNS, ADDSZ_ZERO }, {OP_SETNS, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f9a - SETP */
  {{OP_SETP, ADDSZ_ZERO }, {OP_SETP, ADDSZ_ZERO }, {OP_SETP, ADDSZ_ZERO }, {OP_SETP, ADDSZ_ZERO }, {OP_SETP, ADDSZ_ZERO }, {OP_SETP, ADDSZ_ZERO }, {OP_SETP, ADDSZ_ZERO }, {OP_SETP, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f9b - SETNP */
  {{OP_SETNP, ADDSZ_ZERO }, {OP_SETNP, ADDSZ_ZERO }, {OP_SETNP, ADDSZ_ZERO }, {OP_SETNP, ADDSZ_ZERO }, {OP_SETNP, ADDSZ_ZERO }, {OP_SETNP, ADDSZ_ZERO }, {OP_SETNP, ADDSZ_ZERO }, {OP_SETNP, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f9c - SETL */
  {{OP_SETL, ADDSZ_ZERO }, {OP_SETL, ADDSZ_ZERO }, {OP_SETL, ADDSZ_ZERO }, {OP_SETL, ADDSZ_ZERO }, {OP_SETL, ADDSZ_ZERO }, {OP_SETL, ADDSZ_ZERO }, {OP_SETL, ADDSZ_ZERO }, {OP_SETL, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f9d - SETGE */
  {{OP_SETGE, ADDSZ_ZERO }, {OP_SETGE, ADDSZ_ZERO }, {OP_SETGE, ADDSZ_ZERO }, {OP_SETGE, ADDSZ_ZERO }, {OP_SETGE, ADDSZ_ZERO }, {OP_SETGE, ADDSZ_ZERO }, {OP_SETGE, ADDSZ_ZERO }, {OP_SETGE, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f9e - SETLE */
  {{OP_SETLE, ADDSZ_ZERO }, {OP_SETLE, ADDSZ_ZERO }, {OP_SETLE, ADDSZ_ZERO }, {OP_SETLE, ADDSZ_ZERO }, {OP_SETLE, ADDSZ_ZERO }, {OP_SETLE, ADDSZ_ZERO }, {OP_SETLE, ADDSZ_ZERO }, {OP_SETLE, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f9f - SETG */
  {{OP_SETG, ADDSZ_ZERO }, {OP_SETG, ADDSZ_ZERO }, {OP_SETG, ADDSZ_ZERO }, {OP_SETG, ADDSZ_ZERO }, {OP_SETG, ADDSZ_ZERO }, {OP_SETG, ADDSZ_ZERO }, {OP_SETG, ADDSZ_ZERO }, {OP_SETG, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO fba - GRP8 */
  {{OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_BT, ADDSZ_ZERO }, {OP_BTS, ADDSZ_ZERO }, {OP_BTR, ADDSZ_ZERO }, {OP_BTC, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO fc7 - GRP9 */
  {{OP_INVALID, ADDSZ_ZERO }, {OP_CMPXCHG8B, ADDSZ_TWO}, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }},
  /*  ADDSZ_ZERO f ADDSZ_ZERO  ADDSZ_ONE - GRP7 */
  {{OP_SGDT, ADDSZ_THREE}, {OP_SIDT, ADDSZ_THREE}, {OP_LGDT, ADDSZ_THREE}, {OP_LIDT, ADDSZ_THREE}, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVALID, ADDSZ_ZERO }, {OP_INVLPG, ADDSZ_ZERO }},
};

#define PUSHOP(a,b,c,d,e,f) {b,c,d,e,f}
#define OP_UNSUP OP_INVALID


//Replace 0x00 with macro instance real number in x86ops
#define HEX_0 0x00

static const struct OPCODES x86ops[2][256] = {{
  PUSHOP(HEX_0, ADDR_MRM_GEN_EG, SIZE_BYTE, ADDR_NOADDR, SIZE_NOSIZE, OP_ADD),
  
}};


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



    class disasmx86_controller
    {

        public:
						/*
            inline static void spam_x86(struct DISASMED *s, char *hr);

            inline static const uint8_t *disasm_x86(const uint8_t *command,
                    unsigned int len,
                    struct DISASMED *s);
						*/

            const uint8_t *cli_disasm_one(const uint8_t *buff,
                    unsigned int len,
                    struct DISASM_RESULT *w,
                    int spam);

            int disasmbuf(const uint8_t *buff,
                    unsigned int len,
                    int fd);

    };

}



#endif /* DISASM86_CONTROLLER_HPP */
