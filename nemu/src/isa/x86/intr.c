#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr)
{
    /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
    /*GateDesc32 gate;
  vaddr_t gate_addr = cpu.IDTR.base + NO * 0x8;
  gate.*/
    //TODO();
    rtl_push(s, &cpu.EFLAGS);
    cpu.eflags.IF = 0;
    cpu.eflags.TF = 0;
    rtl_push(s, &cpu.cs);
    rtl_push(s, &cpu.pc);
}

void query_intr(DecodeExecState *s)
{
    TODO();
}
