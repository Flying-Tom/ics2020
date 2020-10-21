#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

char* num_to_str(int x)
{
    char *temp='\0',*out='\0';
    int len=0;
    while(x)
    {
        *temp++=(char)(x%10+'0');
        x/=10;
        len++;
    }
    while (len--)
        *out++=*temp--;
    return out;
}
int _Printf(char *out, const char *fmt, va_list args)
{

    while (*fmt != '\0')
    {
        while (*fmt != '%')
            *out++ = *fmt++;
        fmt++;
        switch (*fmt)
        {
        case 'd':
            strcat(out,num_to_str(va_arg(args, int)));
            break;
        case 's':
            break;
        }
    }
    return 0;
}
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
    va_list ap;
    va_start(ap,fmt);
    int ans = _Printf(out,fmt,ap);
    out[ans]='\0';
    va_end(ap);
    return ans;
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
