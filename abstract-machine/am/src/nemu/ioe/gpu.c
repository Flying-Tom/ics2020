#include <am.h>
#include <nemu.h>
#include <klib.h>

static int W = 400;
static int H = 300;

#define SYNC_ADDR (VGACTL_ADDR + 4)

static inline int min(int x, int y)
{
    return (x < y) ? x : y;
}

void __am_gpu_init()
{
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg)
{
    *cfg = (AM_GPU_CONFIG_T){
        .present = true, .has_accel = false, .width = W, .height = H, .vmemsz = 0};
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl)
{
    if (ctl->sync)
    {
        outl(SYNC_ADDR, 1);
    }
    uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
    int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
    uint32_t *pixels = ctl->pixels;
    int cp_bytes =  min(w, W - x);
    for (int j = 0; j < h && y + j < H; j++)
    {
        //for (int i = 0; i < cp_bytes; i++)
            //*(fb + (y + j) * W + x + i) = *(pixels + i);
        memcpy(fb + (y + j) * W + x, pixels, 5*cp_bytes);
        pixels += w;
    }
}

void __am_gpu_status(AM_GPU_STATUS_T *status)
{
    status->ready = true;
}
