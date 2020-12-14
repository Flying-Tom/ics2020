#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

static int evtdev = -1;
static int fbdev = 5;
static int screen_w = 0, screen_h = 0, canvas_w = 0, canvas_h = 0, space_w = 0, space_h = 0;
static uint32_t *canvas;
static struct timeval boot_time = {.tv_sec = 0, .tv_usec = 0};

uint32_t NDL_GetTicks()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    uint32_t usec = (now.tv_usec - boot_time.tv_usec) % 1000000;
    uint32_t sec = (now.tv_usec - boot_time.tv_usec) / 1000000;
    uint32_t msec = sec * 1000 + usec / 1000;
    return msec;
}

int NDL_PollEvent(char *buf, int len)
{

    memset(buf, '\0', len);
    int events = open("/dev/events", 0, 0);
    read(events, buf, len);
    close(events);
    return strlen(buf);
}

void NDL_Dispinfo_init()
{
    int dispinfo = open("/proc/dispinfo", 0, 0);
    char buf[128];
    read(dispinfo, buf, sizeof(buf));
    sscanf(buf, "WIDTH:%d\nHEIGHT:%d\n", &screen_w, &screen_h);
    close(dispinfo);
    assert(screen_w >= 0 && screen_h >= 0);
}

void NDL_OpenCanvas(int *w, int *h)
{
    NDL_Dispinfo_init();
    if (getenv("NWM_APP"))
    {
        int fbctl = 4;
        fbdev = 5;
        screen_w = *w;
        screen_h = *h;
        char buf[64];
        int len = sprintf(buf, "%d %d", screen_w, screen_h);
        // let NWM resize the window and create the frame buffer
        write(fbctl, buf, len);
        while (1)
        {
            // 3 = evtdev
            int nread = read(3, buf, sizeof(buf) - 1);
            if (nread <= 0)
                continue;
            buf[nread] = '\0';
            if (strcmp(buf, "mmap ok") == 0)
                break;
        }
        close(fbctl);
    }
    else
    {
        if (*w == 0 && *h == 0)
        {
            *w = canvas_w = screen_w;
            *h = canvas_h = screen_h;
        }
        else
        {
            canvas_w = *w;
            canvas_h = *h;
        }
        space_w = (screen_w - canvas_w) / 2;
        space_h = (screen_h - canvas_h) / 2;
        canvas = malloc(sizeof(uint32_t) * canvas_w * canvas_h);
        assert(canvas);
    }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h)
{
    for (int j = 0; j < h; j++)
        for (int i = 0; i < w; i++)
            canvas[(j + y) * canvas_w + (i + x)] = pixels[j * w + i];
    for (int i = 0; i < canvas_h; i++)
    {
        lseek(fbdev, ((i + space_h) * screen_w + space_w) * sizeof(uint32_t), SEEK_SET);
        write(fbdev, &canvas[i * canvas_w], canvas_w);
    }
}

void NDL_OpenAudio(int freq, int channels, int samples)
{
}

void NDL_CloseAudio()
{
}

int NDL_PlayAudio(void *buf, int len)
{
    return 0;
}

int NDL_QueryAudio()
{
    return 0;
}

int NDL_Init(uint32_t flags)
{
    if (getenv("NWM_APP"))
    {
        evtdev = 3;
    }
    return 0;
    gettimeofday(&boot_time, NULL);
    fbdev = open("/dev/fb", 2, 0);
    assert(fbdev == 5);
    close(fbdev);
}

void NDL_Quit()
{
    close(fbdev);
}
