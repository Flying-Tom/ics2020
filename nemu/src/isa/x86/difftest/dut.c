#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"

/*
static void checkregs(CPU_state *ref_r, char *s, bool *judg)
{
    if (concat(cpu., s) != concat(ref_r->, s))
        cnt
}*/

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc)
{
    bool judge = true;
    difftest_check_reg("eax", pc, ref_r->eax, cpu.eax, &judge);
    difftest_check_reg("ecx", pc, ref_r->ecx, cpu.ecx, &judge);
    difftest_check_reg("edx", pc, ref_r->edx, cpu.edx, &judge);
    difftest_check_reg("ebx", pc, ref_r->ebx, cpu.ebx, &judge);
    difftest_check_reg("esp", pc, ref_r->esp, cpu.esp, &judge);
    difftest_check_reg("ebp", pc, ref_r->ebp, cpu.ebp, &judge);
    difftest_check_reg("esi", pc, ref_r->esi, cpu.esi, &judge);
    difftest_check_reg("edi", pc, ref_r->edi, cpu.edi, &judge);
    rtlreg_t* temp = 0 ;
    memcpy(temp,&ref_r->eflags,8UL);
    difftest_check_reg("CF", pc, (*temp >> 0) & 1, cpu.eflags.CF, &judge);
    difftest_check_reg("ZF", pc, (*temp >> 6) & 1, cpu.eflags.ZF, &judge);
    difftest_check_reg("SF", pc, (*temp >> 7) & 1, cpu.eflags.SF, &judge);
    difftest_check_reg("OF", pc, (*temp >> 11) & 1, cpu.eflags.OF, &judge);
    return judge;
}

void isa_difftest_attach()
{
}
