#include "cc.h"
static inline def_EHelper(test)
{
    rtl_and(s, s0, ddest, dsrc1);
    rtl_update_ZFSF(s, s0, id_dest->width);
    rtl_set_CF(s, rz);
    rtl_set_OF(s, rz);
    print_asm_template2(test);
}

static inline def_EHelper(and)
{
    rtl_and(s, s0, ddest, dsrc1);
    operand_write(s, id_dest, s0);
    rtl_update_ZFSF(s, s0, id_dest->width);
    rtl_set_CF(s, rz);
    rtl_set_OF(s, rz);
    print_asm_template2(and);
}

static inline def_EHelper(xor)
{
    rtl_xor(s, s0, ddest, dsrc1);
    operand_write(s, id_dest, s0);
    rtl_update_ZFSF(s, s0, id_dest->width);
    rtl_set_CF(s, rz);
    rtl_set_OF(s, rz);
    print_asm_template2(xor);
}

static inline def_EHelper(or)
{
    rtl_or(s, s0, ddest, dsrc1);
    operand_write(s, id_dest, s0);
    rtl_update_ZFSF(s, s0, id_dest->width);
    rtl_set_CF(s, rz);
    rtl_set_OF(s, rz);
    print_asm_template2(or);
}

static inline def_EHelper(sar)
{
    rtl_sar(s, s0, ddest, dsrc1);
    operand_write(s, id_dest, s0);
    rtl_update_ZFSF(s, s0, id_dest->width);
    // unnecessary to update CF and OF in NEMU
    print_asm_template2(sar);
}

static inline def_EHelper(shl)
{
    rtl_shl(s, s0, ddest, dsrc1);
    operand_write(s, id_dest, s0);
    rtl_update_ZFSF(s, s0, id_dest->width);
    // unnecessary to update CF and OF in NEMU
    print_asm_template2(shl);
}

static inline def_EHelper(shr)
{
    rtl_shr(s, s0, ddest, dsrc1);
    operand_write(s, id_dest, s0);
    rtl_update_ZFSF(s, s0, id_dest->width);
    // unnecessary to update CF and OF in NEMU
    print_asm_template2(shr);
}

static inline def_EHelper(setcc)
{
    uint32_t cc = s->opcode & 0xf;
    rtl_setcc(s, ddest, cc);
    operand_write(s, id_dest, ddest);
    print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

static inline def_EHelper(not )
{
    rtl_not(s, s0, ddest);
    operand_write(s, id_dest, s0);
    print_asm_template1(not );
}

static inline def_EHelper(rol)
{
    rtl_mv(s, s0, ddest);
    rtl_mv(s, s1, dsrc1);
    printf("rol\n");
    while (*s1)
    {
        rtl_msb(s, s2, ddest, id_dest->width);
        rtl_shli(s, s0, s0, 1);
        rtl_add(s, s0, s0, s2);
        rtl_subi(s, s1, s1, 1);
    }
    if (*dsrc1 == 1)
    {
        rtl_msb(s, s1, ddest, id_dest->width);
        rtl_get_CF(s, s2);
        rtl_li(s, s3, s1 != s2 ? 1 : 0);
        rtl_set_OF(s, s3);
    }
    operand_write(s, id_dest, s0);
    print_asm_template2(rol);
}

static inline def_EHelper(ror)
{
    rtl_mv(s, s0, ddest);
    rtl_mv(s, s1, dsrc1);
    while (*s1)
    {
        rtl_andi(s, s2, ddest, 1);
        rtl_shri(s, s0, s0, 1);
        switch (id_dest->width)
        {
        case 1:
            break;
            *s2 = *s2 * 2;
        case 2:
            *s2 = *s2 * 4;
            break;
        case 4:
            *s2 = *s2 * 16;
            break;
        }
        rtl_add(s, s0, s0, s2);
        rtl_subi(s, s1, s1, 1);
    }
    if (*dsrc1 == 1)
    {
        rtl_msb(s, s1, s0, id_dest->width);
        rtl_shri(s, s2, s0, 8 * id_dest->width - 2);
        rtl_li(s, s3, s1 != s2 ? 1 : 0);
        rtl_set_OF(s, s3);
    }
    operand_write(s, id_dest, s0);
    print_asm_template2(rol);
}

static inline def_EHelper(bsr)
{
    if (*dsrc1 == 0)
    {
        rtl_li(s, s3, 1);
        rtl_set_ZF(s, s3);
    }
    else
    {
        rtl_li(s, s3, 0);
        rtl_set_ZF(s, rz);
        rtl_mv(s, s0, dsrc1);
        rtl_li(s, s1, 0);
        while ((*s0 & 1) == 0)
        {
            *s0 >>= 1;
            rtl_addi(s, s1, s1, 1);
        }
        operand_write(s, id_dest, s1);
    }
    print_asm_template2(bsr);
}

static inline def_EHelper(shld)
{
    uint32_t cnt = (uint32_t)*dsrc1;
    cnt = cnt % (id_dest->width * 8);
    rtl_mv(s, s1, ddest);
    rtl_mv(s, s2, dsrc2);
    while (cnt != 0)
    {
        *s0 = 1 & (*s2 >> (id_dest->width * 8 - 1));
        *s1 = (*s1 << 1) + *s0;
        *s2 = *s2 << 1;
        cnt--;
    }
    operand_write(s, id_dest, s1);
    print_asm_template3(shld);
}