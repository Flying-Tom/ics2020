#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <NDL.h>
#include <BMP.h>

void test()
{
    FILE *fp = fopen("/proc/dispinfo", "r");
    char buf[128];
    fread(buf,1,128,fp);
    printf("%s\n",buf);
    fclose(fp);
}

int main()
{
    NDL_Init(0);
    int w, h;
    void *bmp = BMP_Load("/share/pictures/projectn.bmp", &w, &h);
    assert(bmp);
    NDL_OpenCanvas(&w, &h);
    //test(); /////////
    NDL_DrawRect(bmp, 0, 0, w, h);
    free(bmp);
    NDL_Quit();
    printf("Test ends! Spinning...\n");
    while (1)
        ;
    return 0;
}
