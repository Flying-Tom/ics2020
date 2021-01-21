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
    bool keydown = kbd.keydown;
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
    static char dispinfo[256] = {};
    AM_GPU_CONFIG_T gpu_cfg = io_read(AM_GPU_CONFIG);
    sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", gpu_cfg.width, gpu_cfg.height);
    assert(offset == 0);
    strncpy(buf, dispinfo + offset, len);
    return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len)
{
    offset = offset / sizeof(uint32_t);
    len = len / sizeof(uint32_t);
    AM_GPU_CONFIG_T gpu_cfg = io_read(AM_GPU_CONFIG);
    int x = offset % gpu_cfg.width;
    int y = offset / gpu_cfg.width;
    int temp = gpu_cfg.width - x;
    len = len < temp ? len : temp;
    io_write(AM_GPU_FBDRAW, x, y, (void *)buf, len, 1, true);
    return len * sizeof(uint32_t);
}

void init_device()
{
    Log("Initializing devices...");
    ioe_init();
}
