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
    int judge=0;
    difftest_check_reg("eax",pc,ref_r->eax,cpu.eax);
    difftest_check_reg("ecx",pc,ref_r->ecx,cpu.ecx);
    difftest_check_reg("edx",pc,ref_r->edx,cpu.edx);
    difftest_check_reg("ebx",pc,ref_r->ebx,cpu.ebx);
    difftest_check_reg("esp",pc,ref_r->esp,cpu.esp);
    difftest_check_reg("ebp",pc,ref_r->ebp,cpu.ebp);
    difftest_check_reg("esi",pc,ref_r->esi,cpu.esi);
    difftest_check_reg("edi",pc,ref_r->edi,cpu.edi);

    difftest_check_reg("CF",pc,ref_r->eflags.CF,cpu.eflags.CF);
    difftest_check_reg("ZF",pc,ref_r->eflags.ZF,cpu.eflags.ZF);
    difftest_check_reg("SF",pc,ref_r->eflags.SF,cpu.eflags.SF);
    difftest_check_reg("OF",pc,ref_r->eflags.OF,cpu.eflags.OF);
    if (ref_r->eflags.CF != cpu.eflags.CF || ref_r->eflags.ZF != cpu.eflags.ZF || ref_r->eflags.SF != cpu.eflags.SF || ref_r->eflags.OF != cpu.eflags.OF)
    {
        printf("CF: NEMU:%d QEMU:%d\n", cpu.eflags.CF, ref_r->eflags.CF);
        printf("ZF: NEMU:%d QEMU:%d\n", cpu.eflags.ZF, ref_r->eflags.ZF);
        printf("SF: NEMU:%d QEMU:%d\n", cpu.eflags.SF, ref_r->eflags.SF);
        printf("OF: NEMU:%d QEMU:%d\n", cpu.eflags.OF, ref_r->eflags.OF);
        judge=1;
    }
    return (judge==0);
}

void isa_difftest_attach()
{
}
