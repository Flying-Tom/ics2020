#include "cc.h"

static inline def_EHelper(test)
{
    TODO();
    print_asm_template2(test);
}

static inline def_EHelper(and)
{
    TODO();
    print_asm_template2(and);
}

static inline def_EHelper(xor)
{
    rtl_xor(s, s0, ddest, dsrc1);
    operand_write(s, id_dest, s0);
    rtl_sm(s, s0, 0, 0, 4);
    rtl_set_CF(s,s0);
    rtl_set_OF(s,s0);
    print_asm_template2(xor);
}

static inline def_EHelper(or)
{
    TODO();

    print_asm_template2(or);
}

static inline def_EHelper(sar)
{
    TODO();
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
    TODO();
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
