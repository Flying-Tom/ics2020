#ifndef __X86_H__
#define __X86_H__

#define ISA_QEMU_BIN "qemu-system-i386"
#define ISA_QEMU_ARGS

#include "../../../../src/isa/x86/difftest/difftest.h"

/*
union isa_gdb_regs {
  struct {
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t eip, eflags;
    uint32_t cs, ss, ds, es, fs, gs;
  };
  struct {
    uint32_t array[77];
  };
};
*/
union isa_gdb_regs
{
    struct
    {
        uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
        uint32_t eip;
        struct
        {
            uint32_t CF : 1;
            uint32_t : 1;
            uint32_t PF : 1;
            uint32_t : 1;
            uint32_t AF : 1;
            uint32_t : 1;
            uint32_t ZF : 1;
            uint32_t SF : 1;
            uint32_t : 3;
            uint32_t OF : 1;
        } eflags;
        uint32_t cs, ss, ds, es, fs, gs;
    };
    struct
    {
        uint32_t array[77];
    };
};
#endif
