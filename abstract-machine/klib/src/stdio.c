#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
/*
int _Printf(char *s,const char *,va_list args)
{
    
    while(*fmt!='\0')
    {
        while(*fmt!='%')
        *out++=*fmt++;
        fmt++;

    }
}*/
int printf(const char *fmt, ...)
{
    return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap)
{
    return 0;
}

int sprintf(char *out, const char *fmt, ...)
{
    /*va_list ap;
    va_start(ap,fmt);
    int ans = _Printf(s,fmt,ap);
    s[ans]='\0';
    va_end(ap);
    return ans;*/
    return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...)
{
    return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap)
{
    return 0;
}

#endif
