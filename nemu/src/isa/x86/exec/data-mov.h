static inline def_EHelper(mov)
{
    operand_write(s, id_dest, dsrc1);
    print_asm_template2(mov);
}

static inline def_EHelper(push)
{
    if (id_dest->type == OP_TYPE_IMM && id_dest->width == 1)
        rtl_sext(s, ddest, ddest, id_dest->width);
    rtl_push(s, ddest);
    print_asm_template1(push);
}

static inline def_EHelper(pop)
{
    rtl_pop(s, s0);
    operand_write(s, id_dest, s0);
    print_asm_template1(pop);
}

static inline def_EHelper(pusha)
{
    *s0 = reg_l(R_ESP);
    rtl_push(s, &reg_l(R_EAX));
    rtl_push(s, &reg_l(R_ECX));
    rtl_push(s, &reg_l(R_EDX));
    rtl_push(s, &reg_l(R_EBX));
    rtl_push(s, s0);
    rtl_push(s, &reg_l(R_EBP));
    rtl_push(s, &reg_l(R_ESI));
    rtl_push(s, &reg_l(R_EDI));
    print_asm("pusha");
}

static inline def_EHelper(popa)
{

    rtl_pop(s, &reg_l(R_EDI));
    rtl_pop(s, &reg_l(R_ESI));
    rtl_pop(s, &reg_l(R_EBP));
    rtl_pop(s, s0);
    rtl_pop(s, &reg_l(R_EBX));
    rtl_pop(s, &reg_l(R_EDX));
    rtl_pop(s, &reg_l(R_ECX));
    rtl_pop(s, &reg_l(R_EAX));
    print_asm("popa");
}

static inline def_EHelper(leave)
{

    rtl_mv(s, &reg_l(R_ESP), &reg_l(R_EBP));
    rtl_pop(s, &reg_l(R_EBP));
    print_asm("leave");
}

static inline def_EHelper(cltd)
{
    if (s->isa.is_operand_size_16)
    {
        if ((int16_t)reg_w(R_AX) < 0)
            reg_w(R_DX) = 0xffff;
        else
            reg_w(R_DX) = 0;
    }
    else
    {
        if ((int32_t)reg_l(R_EAX) < 0)
            reg_l(R_EDX) = 0xffffffff;
        else
            reg_l(R_EDX) = 0;
    }

    print_asm(s->isa.is_operand_size_16 ? "cwtl" : "cltd");
}

static inline def_EHelper(cwtl)
{
    if (s->isa.is_operand_size_16)
    {
        rtl_lr(s, s0, R_AL, 1);
        rtl_sext(s, s1, s0, 1);
        rtl_sr(s, R_AX, s1, 2);
    }
    else
    {
        rtl_lr(s, s0, R_AX, 2);
        rtl_sext(s, s1, s0, 2);
        rtl_sr(s, R_EAX, s1, 4);
    }

    print_asm(s->isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

static inline def_EHelper(movsx)
{
    id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
    rtl_sext(s, ddest, dsrc1, id_src1->width);
    operand_write(s, id_dest, ddest);
    print_asm_template2(movsx);
}

static inline def_EHelper(movzx)
{
    id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
    operand_write(s, id_dest, dsrc1);
    print_asm_template2(movzx);
}

static inline def_EHelper(movs)
{
    rtl_lr(s, s1, R_ESI, 4);
    rtl_lr(s, s2, R_EDI, 4);
    rtl_lm(s, s0, s1, 0, id_dest->width);
    rtl_sm(s, s2, 0, s0, id_dest->width);
    rtl_lr(s, s0, R_ESI, 4);
    rtl_addi(s, s0, s0, id_dest->width);
    rtl_sr(s, R_ESI, s0, 4);
    rtl_lr(s, s0, R_EDI, 4);
    rtl_addi(s, s0, s0, id_dest->width);
    rtl_sr(s, R_EDI, s0, 4);
    print_asm(str(movs) "%c %%esi,%%edi", suffix_char(id_dest->width), id_src1->str, id_dest->str);
}

static inline def_EHelper(lea)
{
    rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
    operand_write(s, id_dest, ddest);
    print_asm_template2(lea);
}
