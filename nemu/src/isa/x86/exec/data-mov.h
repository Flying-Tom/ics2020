static inline def_EHelper(mov)
{
    operand_write(s, id_dest, dsrc1);
    print_asm_template2(mov);
}

static inline def_EHelper(push)
{
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
    TODO();

    print_asm("pusha");
}

static inline def_EHelper(popa)
{
    TODO();

    print_asm("popa");
}

static inline def_EHelper(leave)
{

    rtl_mv(s, &cpu.esp, &cpu.ebp);
    rtl_pop(s, &cpu.ebp);
    print_asm("leave");
}

static inline def_EHelper(cltd)
{
    if (s->isa.is_operand_size_16)
    {
        if (reg_w(R_AX) < 0)
            reg_w(R_DX) = 0xffff;
        else
            reg_w(R_DX) = 0;
    }
    else
    {
        if (reg_l(R_EAX) < 0)
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
        rtl_sext(s, s1, s0, 2);
        rtl_sr(s, R_AX, s1, 2);
    }
    else
    {
        rtl_lr(s, s0, R_AX, 2);
        rtl_sext(s, s1, s0, 4);
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

static inline def_EHelper(movsb)
{
    if (id_dest->width == 1)
    {
        rtl_lr(s, s0, R_ESI, 1);
        rtl_addi(s,s0,s0,1);
        rtl_sr(s, R_ESI, s0, 1);
        rtl_subi(s,s0,s0,2);
        rtl_sr(s, R_EDI, s0, 1);
        //rtl_mv(s,reg_b(R_EDI),reg_b(R_ESI));
    }
    print_asm_template2(movsb);
}

static inline def_EHelper(lea)
{
    rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
    operand_write(s, id_dest, ddest);
    print_asm_template2(lea);
}
