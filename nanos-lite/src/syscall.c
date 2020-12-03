#include <common.h>
#include "syscall.h"

//extern intptr_t _syscall_(intptr_t, intptr_t, intptr_t, intptr_t);

void do_syscall(Context *c)
{
    uintptr_t a[4];
    a[0] = c->GPR1;
    a[1] = c->GPR2;
    a[2] = c->GPR3;
    a[3] = c->GPR4;

    switch (a[0])
    {
    case SYS_exit:
        halt(0);
        break;
    case SYS_yield:
        //_syscall_(SYS_yield, 0, 0, 0);
        c->GPRx = 0;
        break;
    case SYS_write:
        break;
    default:
        panic("Unhandled syscall ID = %d", a[0]);
    }
}
