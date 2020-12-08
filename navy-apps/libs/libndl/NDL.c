#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0, canvas_w = 0, canvas_h = 0, pad_x = 0, pad_y = 0;
static uint32_t *canvas;

uint32_t NDL_GetTicks()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_usec;
}

int NDL_PollEvent(char *buf, int len)
{

    memset(buf, '\0', len);
    int fp = open("/dev/events", 0, 0);
    read(fp, buf, len);
    close(fp);
    return strlen(buf);
}

void NDL_Dispinfo_init()
{
    FILE *dispinfo = fopen("/proc/dispinfo", "r");
    char buf[128];
    fscanf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", &screen_w, &screen_h);
    fclose(dispinfo);
    assert(screen_w > 0 && screen_h > 0);
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
    {
        if (w == 0 && h == 0)
        {
            canvas_h = screen_h;
            canvas_h = screen_h;
        }
        else
        {
            canvas_w = *w;
            canvas_h = *h;
        }
        canvas = malloc(sizeof(uint32_t) * canvas_w * canvas_h);
        assert(canvas && screen_w >= canvas_w && screen_h >= canvas_h);
        pad_x = (screen_w - canvas_w) / 2;
        pad_y = (screen_w - canvas_w) / 2;
        fbdev = open("/dev/fb", 2, 0);
        assert(fbdev == 5);
    }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h)
{
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            canvas[(i + y) * canvas_w + (j + x)] = pixels[i * w + j];
    for (int i = 0; i < canvas_h; i++)
    {
        lseek(fbdev, ((i + pad_y) * screen_w + pad_x) * sizeof(uint32_t), SEEK_SET);
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
}

void NDL_Quit()
{
}
