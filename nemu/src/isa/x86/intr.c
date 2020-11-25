#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  /*GateDesc32 gate;
  vaddr_t gate_addr = cpu.IDTR.base + NO * 0x8;
  gate.*/
  TODO();
}

void query_intr(DecodeExecState *s) {
  TODO();
}
