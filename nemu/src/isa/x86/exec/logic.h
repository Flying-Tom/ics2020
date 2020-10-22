#include "cc.h"
static inline def_EHelper(test)
{
    rtl_and(s, s0, ddest, dsrc1);
    rtl_update_ZFSF(s, s0, id_dest->width);
    rtl_li(s, s0, 0);
    rtl_set_CF(s, s0);
    rtl_set_OF(s, s0);
    print_asm_template2(test);
}

static inline def_EHelper(and)
{
    if (id_src1->width == 1 && id_dest->width != 1)
    {
        rtl_sext(s, dsrc1, dsrc1, id_src1->width);
        id_src1->width = id_dest->width;
    }
    rtl_and(s, s0, ddest, dsrc1);
    operand_write(s, id_dest, s0);
    rtl_update_ZFSF(s, s0, id_dest->width);
    rtl_li(s, s0, 0);
    rtl_set_CF(s, s0);
    rtl_set_OF(s, s0);
    print_asm_template2(and);
}

static inline def_EHelper(xor)
{
    if (id_src1->width == 1 && id_dest->width != 1)
    {
        rtl_sext(s, dsrc1, dsrc1, id_src1->width);
        id_src1->width = id_dest->width;
    }
    rtl_xor(s, s0, ddest, dsrc1);
    operand_write(s, id_dest, s0);
    rtl_update_ZFSF(s, s0, id_dest->width);
    rtl_li(s, s0, 0);
    rtl_set_CF(s, s0);
    rtl_set_OF(s, s0);
    print_asm_template2(xor);
}

static inline def_EHelper(or)
{
    if (id_src1->width == 1 && id_dest->width != 1)
    {
        rtl_sext(s, dsrc1, dsrc1, id_src1->width);
        id_src1->width = id_dest->width;
    }
    rtl_or(s, s0, ddest, dsrc1);
    operand_write(s, id_dest, s0);
    rtl_update_ZFSF(s, s0, id_dest->width);
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
    rtl_mv(s, s0, dsrc1);
    while(*s0!=0)
    {
        rtl_msb(s,s1,ddest,id_dest->width);
        rtl_add(s,s2,ddest,ddest);
        rtl_add(s,s2,s2,s1);
        rtl_subi(s,s0,s0,1);
    }
    if(*dsrc1==1)
    {
        if(*s1 !=cpu.eflags.CF)
        {
            rtl_li(s,s0,1);
            rtl_set_OF(s,s0);
        }
        else
        {
            rtl_li(s,s0,0);
            rtl_set_OF(s,s0);
        }  
    }
    operand_write(s, id_dest, s2);
    print_asm_template2(rol);
}
