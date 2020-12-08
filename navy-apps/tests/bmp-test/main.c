#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <NDL.h>
#include <BMP.h>

void test()
{
    int WIDTH = 0, HEIGHT = 0;
    FILE *fp = fopen("/proc/dispinfo", "r");
    char buf[128];
    //fread(buf,1,128,fp);
    fscanf(fp, "WIDTH:%d\nHEIGHT:%d\n", &WIDTH, &HEIGHT);
    printf("WIDTH:%d\nHEIGHT:%d\n", WIDTH, HEIGHT);
    fclose(fp);
}

int main()
{
    NDL_Init(0);
    int w, h;
    void *bmp = BMP_Load("/share/pictures/projectn.bmp", &w, &h);
    assert(bmp);
    test();
    NDL_OpenCanvas(&w, &h);
    NDL_DrawRect(bmp, 0, 0, w, h);
    free(bmp);
    NDL_Quit();
    printf("Test ends! Spinning...\n");
    while (1)
        ;
    return 0;
}
