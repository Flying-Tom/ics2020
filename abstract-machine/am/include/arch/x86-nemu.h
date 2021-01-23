#ifndef ARCH_H__
#define ARCH_H__

struct Context
{
  void *cr3;
  uintptr_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  int irq;
  uintptr_t eip, cs, eflags;
};

#define GPR1 eax
#define GPR2 ecx
#define GPR3 edx
#define GPR4 ebx
#define GPRx eax

#endif
