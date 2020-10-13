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
    if(ref_r->eax !=cpu.eax || ref_r->ecx !=cpu.ecx || ref_r->edx !=cpu.edx || ref_r->ebx !=cpu.ebx || ref_r->esp !=cpu.esp || ref_r->ebp !=cpu.ebp || ref_r->esi !=cpu.esi || ref_r->edi !=cpu.edi)
    {
        printf("eax: NEMU:%d QEMU:%d\n",cpu.eax,ref_r->eax);
        printf("ecx: NEMU:%d QEMU:%d\n",cpu.ecx,ref_r->ecx);
        printf("edx: NEMU:%d QEMU:%d\n",cpu.edx,ref_r->edx);
        printf("ebx: NEMU:%d QEMU:%d\n",cpu.ebx,ref_r->ebx);
        printf("esp: NEMU:%d QEMU:%d\n",cpu.esp,ref_r->esp);
        printf("ebp: NEMU:%d QEMU:%d\n",cpu.ebp,ref_r->ebp);
        printf("esi: NEMU:%d QEMU:%d\n",cpu.esi,ref_r->esi);
        printf("edi: NEMU:%d QEMU:%d\n",cpu.edi,ref_r->edi);
        return false;
    }
    else
        return true;
}

void isa_difftest_attach()
{
}
