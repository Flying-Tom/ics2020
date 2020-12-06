#include <common.h>
#include "syscall.h"
#include <fs.h>

extern void __am_timer_init();
extern struct timeval *tv;
extern struct timezone *tz;
extern AM_TIMER_RTC_T rtc;

int sys_gettimeofday(struct timeval *tv, struct timezone *tz)
{
    printf("%llu\n", io_read(AM_TIMER_UPTIME).us);
    return 0;
}

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
        halt(a[1]);
        c->GPRx = 0;
        break;
    case SYS_yield:
        yield();
        c->GPRx = 0;
        break;
    case SYS_open:
        c->GPRx = fs_open((const char *)a[1], a[2], a[3]);
        break;
    case SYS_read:
        c->GPRx = fs_read(a[1], (void *)a[2], a[3]);
        break;
    case SYS_write:
        c->GPRx = fs_write(a[1], (const void *)a[2], a[3]);
        break;
    case SYS_close:
        c->GPRx = fs_close(a[1]);
        break;
    case SYS_lseek:
        c->GPRx = fs_lseek(a[1], a[2], a[3]);
        break;
    case SYS_brk:
        c->GPRx = 0;
        break;
    case SYS_gettimeofday:
        c->GPRx = sys_gettimeofday((struct timeval *)a[1], (struct timezone *)a[2]);
        break;
    default:
        panic("Unhandled syscall ID = %d", a[0]);
    }
}
