#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr)
{
    /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
    assert(NO <= cpu.IDTR.limit);
    vaddr_t gate_addr = cpu.IDTR.base + NO * 8;
    vaddr_t offset_lo = vaddr_read(gate_addr, 4) & 0x0000ffff;
    vaddr_t offset_hi = vaddr_read(gate_addr + 4, 4) & 0xffff0000;
    rtl_push(s, &cpu.EFLAGS);
    rtl_push(s, &cpu.cs);
    rtl_push(s, &ret_addr);
    rtl_j(s, offset_hi | offset_lo);
}

void query_intr(DecodeExecState *s)
{
    TODO();
}
