#include <common.h>
#include <sys/time.h>
#include <fs.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
#define MULTIPROGRAM_YIELD() yield()
#else
#define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
    [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
    [AM_KEY_NONE] = "NONE",
    AM_KEYS(NAME)};

size_t serial_write(const void *buf, size_t offset, size_t len)
{
    for (int i = 0; i < len; i++)
        putch(*(char *)(buf + i));
    return len;
}

size_t events_read(void *buf, size_t offset, size_t len)
{
    AM_INPUT_KEYBRD_T kbd = io_read(AM_INPUT_KEYBRD);
    bool keydown = kbd.keydown; //为什么调用两次ioe_read的结果就会不对
    int keycode = kbd.keycode;
    if (keycode != AM_KEY_NONE)
    {
        if (keydown)
            sprintf(buf, "kd %s", keyname[keycode]);
        else
            sprintf(buf, "ku %s", keyname[keycode]);
    }
    return strlen(buf);
}

size_t dispinfo_read(void *buf, size_t offset, size_t len)
{
    /*
    static char dispinfo[32] = {};

    sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", io_read(AM_GPU_CONFIG).width, io_read(AM_GPU_CONFIG).height);
    printf("WIDTH:%d\nHEIGHT:%d\n", io_read(AM_GPU_CONFIG).width, io_read(AM_GPU_CONFIG).height);*/
    return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len)
{
    return 0;
}

void init_device()
{
    Log("Initializing devices...");
    ioe_init();
}
