#ifndef ARCH_H__
#define ARCH_H__

struct Context
{
  void *cr3;
  uintptr_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uintptr_t irq;
  uintptr_t eip, cs, eflags;
};
/*
struct Context {
  uintptr_t esi, ebx, eax, eip, edx, eflags, ecx, cs, esp, edi, ebp;
  void *cr3;
  int irq;
};
*/

#define GPR1 eax
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eip

#endif
