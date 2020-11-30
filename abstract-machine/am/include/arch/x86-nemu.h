#ifndef ARCH_H__
#define ARCH_H__

struct Context
{
  void *cr3;
  uintptr_t eax, ebx, ecx, edx, esp, ebp, esi, edi;
  int irq;
  uintptr_t eip, eflags, cs;
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
