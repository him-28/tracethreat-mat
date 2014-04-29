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

#include "decompiler/asm_x86/disasmx86_controller.hpp"
#include <stdint.h>

namespace disasms
{

    static void spam_x86(struct DISASMED *s, char *hr)
    {
        int i;
        char comma[2]= {'\0','\0'};

        strcpy(hr, mnemonic[s->real_op]);
        hr += strlen(hr);

        for (i=0; i<3; i++) {
            switch(s->args[i].access) {
            case ACCESS_NOARG:
                break;

            case ACCESS_IMM:
            case ACCESS_REL:
                if (s->args[i].arg.rq >=0)
                    hr += sprintf(hr, "%s %lx", comma, (long)s->args[i].arg.q);
                else
                    hr += sprintf(hr, "%s -%x", comma, -(int)s->args[i].arg.rq);

                break;

            case ACCESS_REG:
                hr += sprintf(hr, "%s %s", comma, x86regs[s->args[i].reg]);
                break;

            case ACCESS_MEM: {
                const char *gotstuff="";
                hr += sprintf(hr, "%s %s ptr ", comma, dis_size[s->args[i].size]);

                if(s->segment) hr += sprintf(hr, "%s:", x86regs[s->segment]);

                *hr++ = '[';
                *hr = '\0';

                if(s->args[i].arg.marg.r1!=REG_INVALID) {
                    switch(s->args[i].arg.marg.scale) {
                    case 1:
                        hr += sprintf(hr, "%s", x86regs[s->args[i].arg.marg.r1]);
                        gotstuff="+";
                        break;

                    case 0:
                        break;

                    default:
                        hr += sprintf(hr, "%s*%d", x86regs[s->args[i].arg.marg.r1],s->args[i].arg.marg.scale);
                        gotstuff="+";
                    }
                }

                if(s->args[i].arg.marg.r2!=REG_INVALID) {
                    hr += sprintf(hr, "%s%s", gotstuff, x86regs[s->args[i].arg.marg.r2]);
                    gotstuff="+";
                }

                if(s->args[i].arg.marg.disp) {
                    if(*gotstuff=='+' && s->args[i].arg.marg.disp<0)
                        hr += sprintf(hr, "-%x", -s->args[i].arg.marg.disp);
                    else
                        hr += sprintf(hr, "%s%x", gotstuff, s->args[i].arg.marg.disp);
                }

                *hr++ = ']';
                *hr = '\0';
            }
            }

            comma[0]=',';
        }
    }


#define INVALIDATE {s->table_op=OP_INVALID; s->state=STATE_ERROR; goto disasm_failed;}
#define GETBYTE(X) if(len--) {X=*command; command++;} else INVALIDATE;
#define GETSIZE(X) (x86ops[table][s->table_op].X!=SIZE_WD?x86ops[table][s->table_op].X:((s->opsize)?SIZE_WORD:SIZE_DWORD))


    static const uint8_t disasm_x86(const uint8_t *command,
            unsigned int len,
            struct DISASMED *s)
    {
        unsigned int reversed=0, i;
        uint8_t b;
        unsigned int table = 0;
        memset(s, 0, sizeof(struct DISASMED));
        s->state = STATE_GETOP;

        while(1) {
            switch (s->state) {
            case STATE_GETOP:
                GETBYTE(s->table_op);
                s->real_op=x86ops[table][s->table_op].op;

                switch(s->real_op) {
                case OP_FPU:
                    s->state = STATE_DECODEX87;
                    break;

                case OP_2BYTE:
                    table=1;
                    break;

                case OP_PREFIX_OPSIZE:
                    s->opsize=1; /* FIXME: check double prefixes */
                    break;

                case OP_PREFIX_ADDRSIZE:
                    s->adsize=1; /* FIXME: check double prefixes */
                    break;

                case OP_PREFIX_SEGMENT:
                    assert(x86ops[table][s->table_op].dmethod>=ADDR_REG_ES && x86ops[table][s->table_op].dmethod<=ADDR_REG_GS);
                    s->segment = regmap[SIZE_WORD][x86ops[table][s->table_op].dmethod];
                    break;

                default:
                    s->state = STATE_CHECKDTYPE;
                }

                continue;

            case STATE_DECODEX87: {
                uint8_t mod;
                uint8_t rm;
                uint8_t rop;
                uint8_t scale;
                uint8_t base;
                uint8_t idx;

                table = s->table_op - 0xd8;
                assert(table<8);
                GETBYTE(rm);

                if (rm>=0xc0) { /* ST */
                    rm&=0x3f;

                    if ((s->real_op=x87_st[table][rm].op)==OP_INVALID) INVALIDATE;

                    switch(x87_st[table][rm].args) {
                    case X87_S:
                        reversed = 1;

                    case X87_R:
                        s->args[reversed^1].access = ACCESS_REG;
                        s->args[reversed^1].reg = REG_ST0;

                    case X87_ONE:
                        s->args[reversed].access = ACCESS_REG;
                        s->args[reversed].reg = static_cast<X86REGS>(REG_ST0 + (rm&7));
                        break;

                    case X87_NONE:
                        break;

                    default:
                        assert("Bad data in x87_st"==0);
                    }

                    s->state = STATE_FINALIZE;
                    continue;
                }

                /* MRM */
                mod=rm>>6;
                rop=(rm>>3)&7;
                rm&=7;

                if ((s->real_op=x87_mrm[table][rop].op)==OP_INVALID) INVALIDATE;

                s->args[0].size = x87_mrm[table][rop].size;
                s->args[0].access = ACCESS_MEM;

                if(!s->adsize) {
                    if (rm==4) {
                        GETBYTE(base);
                        scale=base>>6;
                        idx=(base>>3)&7;
                        base&=7;

                        s->args[0].arg.marg.scale = 1<<scale;

                        if((s->args[0].arg.marg.r2= static_cast<X86REGS>(mrm_regmap[SIZED][base]))==REG_EBP && mod==0) {
                            s->args[0].arg.marg.r2=REG_INVALID;
                            mod=2;
                        }

                        if((s->args[0].arg.marg.r1= static_cast<X86REGS>(mrm_regmap[SIZED][idx]))==REG_ESP) {
                            s->args[0].arg.marg.r1=s->args[0].arg.marg.r2;
                            s->args[0].arg.marg.scale = (s->args[0].arg.marg.r2!=REG_INVALID);
                            s->args[0].arg.marg.r2=REG_INVALID;
                        }
                    } else {
                        if (mod==0 && rm==5) {
                            mod=2;
                            s->args[0].arg.marg.r1=REG_INVALID;
                        } else {
                            s->args[0].arg.marg.scale=1;
                            s->args[0].arg.marg.r1= static_cast<X86REGS>(mrm_regmap[SIZED][rm]);
                        }

                        s->args[0].arg.marg.r2=REG_INVALID;
                    }

                    if(mod==2) mod+=mod;

                    for (i=0; i<mod; i++) {
                        GETBYTE(b);
                        s->args[0].arg.marg.disp+=b<<(i*8);
                        /* FIXME: signextend to dword */
                    }
                } else {
                    if (mod==0 && rm==6) {
                        s->args[0].arg.marg.r1=REG_INVALID;
                        mod=2;
                    } else {
                        s->args[0].arg.marg.scale=1;
                        s->args[0].arg.marg.r1=mrm_regmapw[rm].r1;
                        s->args[0].arg.marg.r2=mrm_regmapw[rm].r2;
                    }

                    for (i=0; i<mod; i++) {
                        GETBYTE(b);
                        s->args[0].arg.marg.disp+=b<<(i*8);
                        /* FIXME: signextend to dword */
                    }
                }

                s->state = STATE_FINALIZE;
                break;
            }

            case STATE_CHECKDTYPE:
                switch(x86ops[table][s->table_op].dmethod) {
                case ADDR_REG_FS:
                case ADDR_REG_GS:

                    /* FIXME: does this exist? */
                case ADDR_REG_ES:
                case ADDR_REG_CS:
                case ADDR_REG_SS:
                case ADDR_REG_DS:
                    assert(x86ops[table][s->table_op].dsize==SIZE_WORD);

                case ADDR_REG_ESP:
                case ADDR_REG_EBP:
                case ADDR_REG_ESI:
                case ADDR_REG_EDI:
                    assert(x86ops[table][s->table_op].dsize!=SIZE_BYTE && x86ops[table][s->table_op].dsize!=SIZE_BYTEH);

                case ADDR_REG_EAX:
                case ADDR_REG_ECX:
                case ADDR_REG_EDX:
                case ADDR_REG_EBX:
                    assert(x86ops[table][s->table_op].dsize<=SIZE_WD);
                    s->args[0].access = ACCESS_REG;
                    s->args[0].reg = static_cast<X86REGS>(regmap[GETSIZE(dsize)][x86ops[table][s->table_op].dmethod]);
                    s->state = STATE_CHECKSTYPE;
                    continue;

                case ADDR_NOADDR:
                    if (x86ops[table][s->table_op].dsize!=SIZE_NOSIZE) {
                        assert(x86ops[table][s->table_op].dsize==SIZE_WD);
                        s->real_op+=(s->opsize!=0);
                    }

                    s->args[0].access = ACCESS_NOARG;
                    s->state = STATE_FINALIZE;
                    continue;

                case ADDR_RELJ:
                case ADDR_IMMED: {
                    uint8_t sz;
                    s->args[0].access = static_cast<DIS_ACCESS>(x86ops[table][s->table_op].dmethod-ADDR_IMMED+ACCESS_IMM);
                    assert(x86ops[table][s->table_op].dsize<SIZE_NOSIZE && s->opsize<2);
                    sz=sizemap[x86ops[table][s->table_op].dsize][s->opsize];
                    assert(sz!=255);
                    s->args[0].size = static_cast<DIS_SIZE>(sz>>1);	/* FIXME: size does matter - 6aff vs 666aff :( */

                    for (i=0; i<sz; i++) {
                        GETBYTE(b);
                        s->args[0].arg.q+=(uint64_t)b<<(i*8);
                    }

                    /* if (x86ops[table][s->table_op].dmethod==ADDR_RELJ) { */
                    s->args[0].arg.q<<=((8-sz)*8);
                    s->args[0].arg.rq>>=((8-sz)*8);
                    /* } */
                    s->state = STATE_CHECKSTYPE;
                    continue;
                }

                case ADDR_MRM_GEN_GE:
                case ADDR_MRM_GEN_GM:
                case ADDR_MRM_GEN_SE:
                case ADDR_MRM_GEN_CR:
                case ADDR_MRM_GEN_DR:
                    reversed = 1;

                case ADDR_MRM_GEN_EG:
                case ADDR_MRM_GEN_ES:
                case ADDR_MRM_EXTRA_1A:
                case ADDR_MRM_EXTRA_1A_M:
                case ADDR_MRM_GEN_RC:
                case ADDR_MRM_GEN_RD: {
                    uint8_t mod;
                    uint8_t rm;
                    uint8_t rop;
                    uint8_t scale;
                    uint8_t base;
                    uint8_t idx;
                    int64_t shiftme=0;
                    const uint8_t (*p)[8];

                    GETBYTE(rm);
                    mod=rm>>6;
                    rop=(rm>>3)&7;
                    rm&=7;

                    switch(x86ops[table][s->table_op].dmethod) {
                    case ADDR_MRM_GEN_RC:
                    case ADDR_MRM_GEN_CR:
                        p = mrm_cregmap;
                        mod = 3;
                        break;

                    case ADDR_MRM_GEN_RD:
                    case ADDR_MRM_GEN_DR:
                        p = mrm_dregmap;
                        mod = 3;
                        break;

                    case ADDR_MRM_GEN_SE:
                    case ADDR_MRM_GEN_ES:
                        p = mrm_sregmap; /* FIXME: CS invalid? */
                        break;

                    default:
                        p = mrm_regmap;
                    }

                    s->args[reversed].size = SIZEB;

                    switch (x86ops[table][s->table_op].dsize) {
                    case SIZE_DWORD:
                        s->args[reversed].size=SIZED;
                        break;

                    case SIZE_WD:
                        s->args[reversed].size = static_cast<DIS_SIZE>(s->args[reversed].size + (s->opsize==0));

                    case SIZE_WORD:
                        s->args[reversed].size = static_cast<DIS_SIZE>(s->args[reversed].size + 1);

                    case SIZE_BYTE:
                        break;

                    default:
                        assert("Bad size"==0);
                    }

                    s->args[reversed^1].access = ACCESS_REG;

                    if ((s->args[reversed^1].reg = static_cast<X86REGS>(p[s->args[reversed].size][rop])) == REG_INVALID) INVALIDATE;

                    /* MOVZX size fixxup */
                    if(s->real_op == OP_MOVZX || s->real_op == OP_MOVSX)
                        s->args[reversed].size = static_cast<DIS_SIZE>(SIZEB + (s->table_op & 1));

                    if(mod==3) {
                        if(x86ops[table][s->table_op].dmethod==ADDR_MRM_GEN_GM 
                             || x86ops[table][s->table_op].dmethod==ADDR_MRM_EXTRA_1A_M) INVALIDATE;

                        s->args[reversed].access = ACCESS_REG;
                        s->args[reversed].reg = static_cast<X86REGS>(mrm_regmap[s->args[reversed].size][rm]);

                        if(x86ops[table][s->table_op].dmethod==ADDR_MRM_EXTRA_1A) {
                            uint8_t opcache = s->real_op;
                            assert(opcache<(sizeof(extra_1a)/sizeof(extra_1a[0][0])));
                            s->args[0].size = static_cast<DIS_SIZE>(s->args[0].size + extra_1a[opcache][rop].addsz);

                            if((s->real_op=extra_1a[opcache][rop].op)==OP_INVALID) INVALIDATE;

                            s->args[1].access=ACCESS_NOARG;

                            if (opcache==6 && rop!=0) {
                                /* f6-f7 - GRP3 - handle test E[bv],I[bv] vs the rest */
                                s->state = STATE_FINALIZE;
                                continue;
                            }
                        } else s->cur++;

                        s->state = STATE_CHECKSTYPE;
                        continue;
                    }

                    s->args[reversed].access = ACCESS_MEM;

                    if(!s->adsize) {
                        if (rm==4) {
                            GETBYTE(base);
                            scale=base>>6;
                            idx=(base>>3)&7;
                            base&=7;

                            s->args[reversed].arg.marg.scale = 1<<scale;

                            if((s->args[reversed].arg.marg.r2=mrm_regmap[SIZED][base])==REG_EBP && mod==0) {
                                s->args[reversed].arg.marg.r2=REG_INVALID;
                                mod=2;
                            }

                            if((s->args[reversed].arg.marg.r1=mrm_regmap[SIZED][idx])==REG_ESP) {
                                s->args[reversed].arg.marg.r1=s->args[reversed].arg.marg.r2;
                                s->args[reversed].arg.marg.scale = (s->args[reversed].arg.marg.r2!=REG_INVALID);
                                s->args[reversed].arg.marg.r2=REG_INVALID;
                            }
                        } else {
                            if (mod==0 && rm==5) {
                                mod=2;
                                s->args[reversed].arg.marg.r1=REG_INVALID;
                            } else {
                                s->args[reversed].arg.marg.scale=1;
                                s->args[reversed].arg.marg.r1 = mrm_regmap[SIZED][rm];
                            }

                            s->args[reversed].arg.marg.r2=REG_INVALID;
                        }

                        if(mod==2) mod+=mod;

                        for (i=0; i<mod; i++) {
                            GETBYTE(b);
                            shiftme+=b<<(i*8);
                        }

                        if(mod) {
                            shiftme<<=((8-mod)*8);
                            s->args[reversed].arg.marg.disp=shiftme>>((8-mod)*8);
                        } else s->args[reversed].arg.marg.disp=0;
                    } else {
                        if (mod==0 && rm==6) {
                            s->args[reversed].arg.marg.r1=REG_INVALID;
                            mod=2;
                        } else {
                            s->args[reversed].arg.marg.scale=1;
                            s->args[reversed].arg.marg.r1=mrm_regmapw[rm].r1;
                            s->args[reversed].arg.marg.r2=mrm_regmapw[rm].r2;
                        }

                        for (i=0; i<mod; i++) {
                            GETBYTE(b);
                            shiftme+=b<<(i*8);
                        }

                        shiftme<<=((8-mod)*8);
                        s->args[reversed].arg.marg.disp=shiftme>>((8-mod)*8);
                    }

                    if(x86ops[table][s->table_op].dmethod==ADDR_MRM_EXTRA_1A || x86ops[table][s->table_op].dmethod==ADDR_MRM_EXTRA_1A_M) {
                        uint8_t opcache = s->real_op;
                        assert(opcache<(sizeof(extra_1a)/sizeof(extra_1a[0][0])));
                        s->args[0].size+=extra_1a[opcache][rop].addsz;

                        if ((s->real_op=extra_1a[opcache][rop].op)==OP_INVALID) INVALIDATE;

                        s->args[1].access=ACCESS_NOARG;

                        if (opcache==6 && rop!=0) {
                            /* f6-f7 - GRP3 - handle test E[bv],I[bv] vs the rest */
                            s->state = STATE_FINALIZE;
                            continue;
                        }
                    } else s->cur++;

                    s->state = STATE_CHECKSTYPE;
                    continue;
                }

                case ADDR_OFFSET: {
                    uint8_t sz;
                    s->args[0].access = ACCESS_MEM;
                    assert((x86ops[table][s->table_op].dsize==SIZE_BYTE || x86ops[table][s->table_op].dsize==SIZE_WD) && s->adsize<2);
                    sz=sizemap[SIZE_WD][s->adsize];
                    assert(sz!=255);
                    assert(s->opsize<2);
                    s->args[0].size=sizemap[x86ops[table][s->table_op].dsize][s->opsize];
                    assert(s->args[0].size!=255);
                    s->args[0].size>>=1;
                    s->args[0].arg.marg.r1=REG_INVALID;
                    s->args[0].arg.marg.r2=REG_INVALID;

                    for (i=0; i<sz; i++) {
                        GETBYTE(b);
                        s->args[0].arg.marg.disp+=b<<(i*8);
                    }

                    s->state = STATE_CHECKSTYPE;
                    continue;
                }

                default:
                    assert("BAD DST METHOD"==0);
                }


            case STATE_CHECKSTYPE:
                s->cur++;

                switch(x86ops[table][s->table_op].smethod) {
                case ADDR_NOADDR:
                    s->state = STATE_FINALIZE;
                    continue;

                case ADDR_IMMED: {
                    uint8_t sz;
                    s->args[s->cur].access = ACCESS_IMM;
                    assert((x86ops[table][s->table_op].ssize==SIZE_WD || x86ops[table][s->table_op].ssize==SIZE_BYTE) && s->opsize<2);
                    sz=sizemap[x86ops[table][s->table_op].ssize][s->opsize];
                    s->args[s->cur].size = sz>>1;

                    for (i=0; i<sz; i++) {
                        GETBYTE(b);
                        s->args[s->cur].arg.q+=b<<(i*8);
                    }

                    s->args[s->cur].arg.q<<=((8-sz)*8);
                    s->args[s->cur].arg.rq>>=((8-sz)*8);
                    s->state = STATE_FINALIZE;
                    continue;
                }

                case ADDR_REG_EAX:
                case ADDR_REG_ECX:
                case ADDR_REG_EDX:
                    assert(x86ops[table][s->table_op].ssize<=SIZE_WD);
                    s->args[s->cur].access = ACCESS_REG;
                    s->args[s->cur].reg = regmap[GETSIZE(ssize)][x86ops[table][s->table_op].smethod];
                    s->state = STATE_FINALIZE;
                    continue;

                case ADDR_IMPLICIT:
                    s->args[s->cur].access = ACCESS_IMM;
                    s->args[s->cur].size = 1;
                    s->args[s->cur].arg.q = 1;
                    s->state = STATE_FINALIZE;
                    continue;

                case ADDR_OFFSET: {
                    uint8_t sz;
                    s->args[1].access = ACCESS_MEM;
                    assert((x86ops[table][s->table_op].ssize==SIZE_BYTE || x86ops[table][s->table_op].ssize==SIZE_WD) && s->adsize<2);
                    sz=sizemap[SIZE_WD][s->adsize];
                    assert(sz!=255);
                    assert(s->opsize<2);
                    s->args[1].size=sizemap[x86ops[table][s->table_op].ssize][s->opsize];
                    assert(s->args[1].size!=255);
                    s->args[1].size>>=1;
                    s->args[1].arg.marg.r1=REG_INVALID;
                    s->args[1].arg.marg.r2=REG_INVALID;

                    for (i=0; i<sz; i++) {
                        GETBYTE(b);
                        s->args[1].arg.marg.disp+=b<<(i*8);
                    }

                    s->state = STATE_FINALIZE;
                    continue;
                }

                default:
                    assert("BAD SRC METHOD"==0);
                }

            case STATE_ERROR:
disasm_failed:
                return NULL;

            case STATE_FINALIZE:
                if (s->real_op==OP_INVALID) INVALIDATE;

                return command;

            default:
                assert("INVALID STATE"==0);
            }
        }
    }

    const uint8_t *disasmx86_controller::cli_disasm_one(const uint8_t *buff,
            unsigned int len,
            struct DISASM_RESULT *w, int spam)
    {
        struct DISASMED s;
        int i;

        memset(&w->extra[0], 0, sizeof(w->extra));
        buff = disasm_x86(buff, len, &s);

        if (!buff)
            return NULL;

        if (spam) {
            char hr[128];
            spam_x86(&s, hr);
           // cli_dbgmsg("%s\n", hr);
        }

        w->real_op = le16_to_host(s.real_op);
        w->opsize = s.opsize;
        w->adsize = s.adsize;
        w->segment = s.segment;

        for (i=0; i<3; i++) {
            w->arg[i][0] = s.args[i].access;
            w->arg[i][1] = s.args[i].size;

            switch(s.args[i].access) {
            case ACCESS_MEM:
                w->arg[i][2]=s.args[i].arg.marg.r1;
                w->arg[i][3]=s.args[i].arg.marg.r2;
                w->arg[i][4]=s.args[i].arg.marg.scale;
                w->arg[i][5]=0;
                cli_writeint32(&w->arg[i][6], s.args[i].arg.marg.disp);
                break;

            case ACCESS_REG:
                w->arg[i][1] = s.args[i].reg;

            default:
                cli_writeint32(&w->arg[i][2], s.args[i].arg.q);
                cli_writeint32(&w->arg[i][6], s.args[i].arg.q>>32);
            }
        }

        return buff;
    }

    int disasmx86_controller::disasmbuf(const uint8_t *buff, unsigned int len, int fd)
    {
        const uint8_t *next = buff;
        unsigned int counter=0;
        int gotsome=0;
        struct DISASM_RESULT w;
        memset(&w.extra[0], 0, sizeof(w.extra));

        while(len && counter++<200) {
            if(!(next = cli_disasm_one(next, len, &w, 1))) {
                /* TODO: invd opcode or buff over */
                return gotsome;
            }

            len -= next-buff;
            buff=next;

            //cli_writen(fd, &w, sizeof(w));
            gotsome = 1;
        }

        return gotsome;
    }

}


