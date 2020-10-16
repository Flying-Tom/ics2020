#include "cc.h"

static inline def_EHelper(test)
{
    rtl_and(s, s0, ddest, dsrc1);
    operand_write(s, id_dest, s0);
    rtl_update_ZFSF(s, ddest, id_dest->width);
    rtl_li(s, s0, 0);
    rtl_set_CF(s, s0);
    rtl_set_OF(s, s0);
    print_asm_template2(test);
}

static inline def_EHelper(and)
{
    if (id_src1->width == 1)
    {
        rtl_sext(s, dsrc1, dsrc1, id_src1->width);
        s->width = id_dest->width = id_src1->width = 4;
    }
    rtl_and(s, s0, ddest, dsrc1);
    operand_write(s, id_dest, s0);
    rtl_update_ZFSF(s, ddest, id_dest->width);
    /*
    *s0 = (s0==0)? 1:0;
    rtl_set_ZF(s,s0);
    rtl_msb(s,s0,ddest,id_dest->width);
    rtl_set_SF(s,s0);
    *s0=0;*/
    rtl_li(s, s0, 0);
    rtl_set_CF(s, s0);
    rtl_set_OF(s, s0);
    print_asm_template2(and);
}

static inline def_EHelper(xor)
{
    rtl_xor(s, s0, ddest, dsrc1);
    operand_write(s, id_dest, s0);
    /*
    *s0 = (s0==0)? 1:0;
    rtl_set_ZF(s,s0);
    rtl_msb(s,s0,ddest,id_dest->width);
    rtl_set_SF(s,s0);
    *s0=0;*/
    rtl_update_ZFSF(s, s0, id_dest->width);
    rtl_li(s, s0, 0);
    rtl_set_CF(s, s0);
    rtl_set_OF(s, s0);
    print_asm_template2(xor);
}

static inline def_EHelper(or)
{
    rtl_or(s, s0, ddest, dsrc1);
    operand_write(s, id_dest, s0);
    rtl_li(s, s0, 0);
    rtl_set_CF(s, s0);
    rtl_set_OF(s, s0);
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
    TODO();
    // unnecessary to update CF and OF in NEMU

    print_asm_template2(shl);
}

static inline def_EHelper(shr)
{
    rtl_mv(s, s3, dsrc1);
    rtl_li(s, s2, 2);
    while (*s3 != 0)
    {
        rtl_div_q(s, ddest, ddest, s2);
        *s3 -= 1;
    }
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
    TODO();

    print_asm_template1(not );
}
