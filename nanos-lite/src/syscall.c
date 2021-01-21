#include <common.h>
#include "syscall.h"
#include <fs.h>
#include <sys/time.h>
#include <proc.h>

extern void naive_uload(PCB *, const char *);

int sys_gettimeofday(struct timeval *tv, struct timezone *tz)
{
    uint64_t us = io_read(AM_TIMER_UPTIME).us;
    tv->tv_sec = us / 1000000;
    tv->tv_usec = us % 1000000;
    return 0;
}

int sys_execve(const char *fname, char *const argv[], char *const envp[])
{
    naive_uload(NULL, fname);
    return -1;
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
        //halt(a[1]);
        c->GPRx = sys_execve("/bin/menu", NULL, NULL);
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
    case SYS_execve:
        c->GPRx = sys_execve((char *)a[1], (char **)a[2], (char **)a[3]);
        break;
    default:
        panic("Unhandled syscall ID = %d", a[0]);
        break;
    }
}
