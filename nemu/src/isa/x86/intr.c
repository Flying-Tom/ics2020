#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr)
{
    /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
    printf("cpu.IDTR.limit:%d\n",cpu.IDTR.limit);
    //assert(NO<=cpu.IDTR.limit);
    vaddr_t gate_addr = cpu.IDTR.base + NO * 0x8;
    vaddr_t gate_selector = vaddr_read(gate_addr, 4) & 0x0000ffff;
    vaddr_t gate_offset = vaddr_read(gate_addr + 4, 4) & 0xffff0000;
    rtl_push(s, &cpu.EFLAGS);
    cpu.eflags.IF = 0;
    cpu.eflags.TF = 0;
    rtl_push(s, &cpu.cs);
    rtl_push(s, &ret_addr);
    rtl_j(s, gate_offset | gate_selector);
}

void query_intr(DecodeExecState *s)
{
    TODO();
}
