#include <cpu/exec.h>
#include "../local-include/decode.h"
#include "all-instr.h"

static inline void set_width(DecodeExecState *s, int width)
{
    if (width == -1)
        return;
    if (width == 0)
    {
        width = s->isa.is_operand_size_16 ? 2 : 4;
    }
    s->src1.width = s->dest.width = s->src2.width = width;
}

/* 0x80, 0x81, 0x83 */
static inline def_EHelper(gp1)
{
    switch (s->isa.ext_opcode)
    {
        EX(0, add)
        EMPTY(1)
        EMPTY(2)
        EMPTY(3)
        EXW(4, and, 1) //////////////
        EX(5, sub)     //EXW(5,sub,1)?
        EMPTY(6)       //////////////
        EXW(7, cmp, 1)
    }
}

/* 0xc0, 0xc1, 0xd0, 0xd1, 0xd2, 0xd3 */
static inline def_EHelper(gp2)
{
    switch (s->isa.ext_opcode)
    {
        EMPTY(0)
        EMPTY(1)
        EMPTY(2)
        EMPTY(3)
        EMPTY(4)
        EMPTY(5)
        EMPTY(6)
        EMPTY(7)
    }
}

/* 0xf6, 0xf7 */
static inline def_EHelper(gp3)
{
    switch (s->isa.ext_opcode)
    {
        EMPTY(0)
        EMPTY(1)
        EMPTY(2)
        EMPTY(3)
        EMPTY(4)
        EMPTY(5)
        EMPTY(6)
        EMPTY(7)
    }
}

/* 0xfe */
static inline def_EHelper(gp4)
{
    switch (s->isa.ext_opcode)
    {
        EMPTY(0)
        EMPTY(1)
        EMPTY(2)
        EMPTY(3)
        EMPTY(4)
        EMPTY(5)
        EMPTY(6)
        EMPTY(7)
    }
}

/* 0xff */
static inline def_EHelper(gp5)
{
    switch (s->isa.ext_opcode)
    {
        EX(0, inc)
        EMPTY(1)
        EMPTY(2)
        EMPTY(3)
        EMPTY(4)
        EMPTY(5)
        //EMPTY(6)
        EX(6, push)
        EMPTY(7)
    }
}

/* 0x0f 0x01*/
static inline def_EHelper(gp7)
{
    switch (s->isa.ext_opcode)
    {
        EMPTY(0)
        EMPTY(1)
        EMPTY(2)
        EMPTY(3)
        EMPTY(4)
        EMPTY(5)
        EMPTY(6)
        EMPTY(7)
    }
}

static inline def_EHelper(2byte_esc)
{
    uint8_t opcode = instr_fetch(&s->seq_pc, 1);
    s->opcode = opcode;
    switch (opcode)
    {
        /* TODO: Add more instructions!!! */
        IDEX(0x01, gp7_E, gp7)
        IDEXW(0x94, setcc_E, setcc, 1)
        IDEX(0xb6, movx1_G2E, movzx)
        IDEX(0xb7, movx2_G2E, movzx)
        IDEX(0xaf, E2G, imul2)
    default:
        exec_inv(s);
    }
}

static inline void fetch_decode_exec(DecodeExecState *s)
{
    uint8_t opcode;
again:
    opcode = instr_fetch(&s->seq_pc, 1);
    s->opcode = opcode;
    switch (opcode)
    {
        EX(0x0f, 2byte_esc)
        IDEXW(0x80, I2E, gp1, 1)
        IDEX(0x81, I2E, gp1)
        IDEX(0x83, SI2E, gp1)
        IDEXW(0x88, mov_G2E, mov, 1)
        IDEX(0x89, mov_G2E, mov)
        IDEXW(0x8a, mov_E2G, mov, 1)
        IDEX(0x8b, mov_E2G, mov)
        IDEXW(0xa0, O2a, mov, 1)
        IDEX(0xa1, O2a, mov)
        IDEXW(0xa2, a2O, mov, 1)
        IDEX(0xa3, a2O, mov)
        IDEXW(0xb0, mov_I2r, mov, 1)
        IDEXW(0xb1, mov_I2r, mov, 1)
        IDEXW(0xb2, mov_I2r, mov, 1)
        IDEXW(0xb3, mov_I2r, mov, 1)
        IDEXW(0xb4, mov_I2r, mov, 1)
        IDEXW(0xb5, mov_I2r, mov, 1)
        IDEXW(0xb6, mov_I2r, mov, 1)
        IDEXW(0xb7, mov_I2r, mov, 1)
        IDEX(0xb8, mov_I2r, mov)
        IDEX(0xb9, mov_I2r, mov)
        IDEX(0xba, mov_I2r, mov)
        IDEX(0xbb, mov_I2r, mov)
        IDEX(0xbc, mov_I2r, mov)
        IDEX(0xbd, mov_I2r, mov)
        IDEX(0xbe, mov_I2r, mov)
        IDEX(0xbf, mov_I2r, mov)
        IDEXW(0xc0, gp2_Ib2E, gp2, 1)
        IDEX(0xc1, gp2_Ib2E, gp2)
        IDEXW(0xc6, mov_I2E, mov, 1)
        IDEX(0xc7, mov_I2E, mov)
        IDEXW(0xd0, gp2_1_E, gp2, 1)
        IDEX(0xd1, gp2_1_E, gp2)
        IDEXW(0xd2, gp2_cl2E, gp2, 1)
        IDEX(0xd3, gp2_cl2E, gp2)
        EX(0xd6, nemu_trap)
        IDEXW(0xf6, E, gp3, 1)
        IDEX(0xf7, E, gp3)
        IDEXW(0xfe, E, gp4, 1)
        ///////////////////
        IDEX(0xff, E, gp5)
        ///////////////////
        IDEX(0xe8, J, call)
        ///////////////////
        IDEX(0x50, r, push)
        IDEX(0x51, r, push)
        IDEX(0x52, r, push)
        IDEX(0x53, r, push)
        IDEX(0x54, r, push)
        IDEX(0x55, r, push)
        IDEX(0x56, r, push)
        IDEX(0x57, r, push)
        IDEX(0x58, r, pop)
        IDEX(0x59, r, pop)
        IDEX(0x5a, r, pop)
        IDEX(0x5b, r, pop)
        IDEX(0x5c, r, pop)
        IDEX(0x5d, r, pop)
        IDEX(0x5e, r, pop)
        IDEX(0x5f, r, pop)
        IDEXW(0x6a, I, push, 1)
        IDEX(0x68, I, push)
        ///////////////////
        IDEX(0x09, G2E, or)
        //////////////////
        IDEX(0x30, G2E, xor)
        IDEX(0x31, G2E, xor)
        IDEXW(0x32, E2G, xor, 1) // XOR r8,r/m8
        IDEX(0x33, E2G, xor)
        IDEX(0x34, I2a, xor)
        IDEX(0x35, I2a, xor)
        ///////////////////
        IDEX(0x8d, lea_M2G, lea)
        ///////////////////
        IDEX(0x01, G2E, add)
        IDEX(0x03, mov_E2G, add)
        ///////////////////
        IDEX(0x3b, E2G, cmp)
        IDEX(0x39, E2G, cmp)
        IDEX(0x3d, I2r, cmp)
        ///////////////////
        IDEX(0x13, E2G, adc)
        ///////////////////
        IDEXW(0x74, J, jcc, 1)
        IDEXW(0x75, J, jcc, 1)
        IDEXW(0x7d, J, jcc, 1)
        IDEXW(0x7e, J, jcc, 1)
        IDEXW(0xeb, J, jmp, 1)
        EX(0xc9, leave)
        EX(0xc3, ret)
        EX(0x90, nop)
        ///////////////////
        IDEX(0x40, r, inc)
        IDEX(0x41, r, inc)
        IDEX(0x42, r, inc)
        IDEX(0x43, r, inc)
        IDEX(0x44, r, inc)
        IDEX(0x45, r, inc)
        IDEX(0x46, r, inc)
        IDEX(0x47, r, inc)
        IDEX(0x48, r, dec)
        IDEX(0x49, r, dec)
        IDEX(0x4a, r, dec)
        IDEX(0x4b, r, dec)
        IDEX(0x4c, r, dec)
        IDEX(0x4d, r, dec)
        IDEX(0x4e, r, dec)
        IDEX(0x4f, r, dec)
        //////////////////
        EX(0x99,cltd)
    case 0x66:
        s->isa.is_operand_size_16 = true;
        goto again;
    default:
        exec_inv(s);
    }
}

vaddr_t isa_exec_once()
{
    DecodeExecState s;
    s.is_jmp = 0;
    s.isa = (ISADecodeInfo){0};
    s.seq_pc = cpu.pc;

    fetch_decode_exec(&s);
    update_pc(&s);

    return s.seq_pc;
}
