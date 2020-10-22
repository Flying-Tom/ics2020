#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int num_to_str(char *out, char *ctrl, int x)
{
    char temp[32];
    int len = 0, ans = 0;
    while (x)
    {
        temp[len++] = x % 10 + '0';
        x /= 10;
    }
    ans = len;
    len--;
    /*
    if (ctrl[0] == '0' && ctrl[1] != '\0')
        while (len < ctrl[1] - 49)
            temp[len++] = '0';
    len--;
    memset(ctrl, '\0', sizeof(ctrl));*/
    while (len > 0)
        *out++ = temp[len--];
    return ans;
}

int _Printf(char *out, const char *fmt, va_list args)
{

    int ans = 0, temp = 0;
    char *strtemp = '\0';
    char ctrl[16];
    while (*fmt != '\0')
    {
        while (*fmt != '%' && *fmt != '\0')
        {
            *out++ = *fmt++;
            ans++;
        }
        if (*fmt++ == '\0')
            break;
        while (*fmt >= '0' && *fmt <= '9')
            fmt++;

        switch (*fmt++)
        {
        case 'd':
            temp = num_to_str(out, ctrl, va_arg(args, int));
            ans += temp;
            out += temp;
            break;
        case 'c':
            *strtemp = va_arg(args, int);
            *out++ = *strtemp++;
            ans++;
            break;
        case 's':
            strtemp = va_arg(args, char *);
            while (*strtemp)
            {
                *out++ = *strtemp++;
                ans++;
            }
            break;
        }
    }
    return ans;
}

int printf(const char *fmt, ...)
{
    va_list ap;
    char buf[256];
    va_start(ap, fmt);
    int ans = _Printf(buf, fmt, ap);
    buf[ans] = '\0';
    va_end(ap);
    size_t cnt = 0;
    while (buf[cnt])
        putch(buf[cnt++]);
    return ans;
}

int vsprintf(char *out, const char *fmt, va_list ap)
{
    return 0;
}

int sprintf(char *out, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ans = _Printf(out, fmt, ap);
    out[ans] = '\0';
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
