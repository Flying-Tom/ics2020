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
    bool judge=true;
    difftest_check_reg("eax",pc,ref_r->eax,cpu.eax, &judge);
    difftest_check_reg("ecx",pc,ref_r->ecx,cpu.ecx, &judge);
    difftest_check_reg("edx",pc,ref_r->edx,cpu.edx, &judge);
    difftest_check_reg("ebx",pc,ref_r->ebx,cpu.ebx, &judge);
    difftest_check_reg("esp",pc,ref_r->esp,cpu.esp, &judge);
    difftest_check_reg("ebp",pc,ref_r->ebp,cpu.ebp, &judge);
    difftest_check_reg("esi",pc,ref_r->esi,cpu.esi, &judge);
    difftest_check_reg("edi",pc,ref_r->edi,cpu.edi, &judge);

    difftest_check_reg("CF",pc,ref_r->eflags.CF,cpu.eflags.CF, &judge);
    difftest_check_reg("ZF",pc,ref_r->eflags.ZF,cpu.eflags.ZF, &judge);
    difftest_check_reg("SF",pc,ref_r->eflags.SF,cpu.eflags.SF, &judge);
    difftest_check_reg("OF",pc,ref_r->eflags.OF,cpu.eflags.OF, &judge);
    return judge;
}

void isa_difftest_attach()
{
}
