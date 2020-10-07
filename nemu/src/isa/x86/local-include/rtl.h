#ifndef __X86_RTL_H__
#define __X86_RTL_H__

#include <rtl/rtl.h>
#include "reg.h"

/* RTL pseudo instructions */

static inline def_rtl(lr, rtlreg_t *dest, int r, int width)
{
    switch (width)
    {
    case 4:
        rtl_mv(s, dest, &reg_l(r));
        return;
    case 1:
        rtl_host_lm(s, dest, &reg_b(r), 1);
        return;
    case 2:
        rtl_host_lm(s, dest, &reg_w(r), 2);
        return;
    default:
        assert(0);
    }
}

static inline def_rtl(sr, int r, const rtlreg_t *src1, int width)
{
    switch (width)
    {
    case 4:
        rtl_mv(s, &reg_l(r), src1);
        return;
    case 1:
        rtl_host_sm(s, &reg_b(r), src1, 1);
        return;
    case 2:
        rtl_host_sm(s, &reg_w(r), src1, 2);
        return;
    default:
        assert(0);
    }
}

static inline def_rtl(push, const rtlreg_t *src1)
{
    // esp <- esp - 4
    // M[esp] <- src1
    if (s->isa.is_operand_size_16)
    {
        rtl_subi(s, &cpu.esp, &cpu.esp, 2);
        rtl_sm(s, &cpu.esp, 0, src1, 2);
    }
    else
    {
        rtl_subi(s, &cpu.esp, &cpu.esp, 4);
        rtl_sm(s, &cpu.esp, 0, src1, 4);
    }
}

static inline def_rtl(pop, rtlreg_t *dest)
{
    // dest <- M[esp]
    // esp <- esp + 4
    if (s->isa.is_operand_size_16)
    {
        rtl_lm(s, dest, &cpu.esp, 0, 2);
        rtl_addi(s, &cpu.esp, &cpu.esp, 2);
    }
    else
    {
        rtl_lm(s, dest, &cpu.esp, 0, 4);
        rtl_addi(s, &cpu.esp, &cpu.esp, 4);
    }
}

static inline rtlreg_t is_overflow(const rtlreg_t *src1, char op, const rtlreg_t *src2)
{
    /*rtlreg_t res;
    switch (op)
    {
    case '+':
        res = *src1 + *src2;
        break;
    case '-':
        res = *src1 - *src2;
        break;
    }*/
    return 0;
}

static inline rtlreg_t is_carry(const rtlreg_t *src1, char op, const rtlreg_t *src2)
{
    return 1;
}

static inline def_rtl(is_sub_overflow, rtlreg_t *dest,
                      const rtlreg_t *res, const rtlreg_t *src1, const rtlreg_t *src2, int width)
{
    // dest <- is_overflow(src1 - src2)
    *dest = is_overflow(src1, '-', src2);
}

static inline def_rtl(is_sub_carry, rtlreg_t *dest,
                      const rtlreg_t *src1, const rtlreg_t *src2)
{
    // dest <- is_carry(src1 - src2)
    *dest = is_carry(src1, '-', src2);
}

static inline def_rtl(is_add_overflow, rtlreg_t *dest,
                      const rtlreg_t *res, const rtlreg_t *src1, const rtlreg_t *src2, int width)
{
    // dest <- is_overflow(src1 + src2)
    *dest = is_overflow(src1, '+', src2);
}

static inline def_rtl(is_add_carry, rtlreg_t *dest,
                      const rtlreg_t *res, const rtlreg_t *src1)
{
    // dest <- is_carry(src1 + src2)
    *dest = is_carry(src1, '+', res);
}

#define def_rtl_setget_eflags(f)                                \
    static inline def_rtl(concat(set_, f), const rtlreg_t *src) \
    {                                                           \
        cpu.eflags.f = *src;                                    \
    }                                                           \
    static inline def_rtl(concat(get_, f), rtlreg_t *dest)      \
    {                                                           \
        *dest = cpu.eflags.f;                                   \
    }

def_rtl_setget_eflags(CF)
    def_rtl_setget_eflags(OF)
        def_rtl_setget_eflags(ZF)
            def_rtl_setget_eflags(SF)

                static inline def_rtl(update_ZF, const rtlreg_t *result, int width)
{
    // eflags.ZF <- is_zero(result[width * 8 - 1 .. 0])
    cpu.eflags.ZF = (*result == 0);
}

static inline def_rtl(update_SF, const rtlreg_t *result, int width)
{
    // eflags.SF <- is_sign(result[width * 8 - 1 .. 0])
    rtl_shri(s, &cpu.eflags.SF, result, width * 8 - 1);
}

static inline def_rtl(update_ZFSF, const rtlreg_t *result, int width)
{
    rtl_update_ZF(s, result, width);
    rtl_update_SF(s, result, width);
}

#endif
