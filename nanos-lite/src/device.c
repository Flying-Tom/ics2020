#include <common.h>
#include <sys/time.h>

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
    int keycode = io_read(AM_INPUT_KEYBRD).keycode;
    bool keydown = io_read(AM_INPUT_KEYBRD).keydown;
    if (keydown)
    {
        //sprintf(buf, "kd %s\n", keyname[keycode]);
    }
    else if (keycode != AM_KEY_NONE)
    {
        //sprintf(buf, "kd RETURN\n");
    }

    return strlen(buf);
}

size_t dispinfo_read(void *buf, size_t offset, size_t len)
{
    return 0;
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
