#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <NDL.h>
#include <BMP.h>

int main()
{
    NDL_Init(0);
    int w, h;
    uint32_t *test = malloc(128 * sizeof(uint32_t));
    assert(test);
    void *bmp = BMP_Load("/share/pictures/projectn.bmp", &w, &h);
    assert(bmp);
    NDL_OpenCanvas(&w, &h);
    NDL_DrawRect(bmp, 0, 0, w, h);
    free(bmp);
    NDL_Quit();
    printf("Test ends! Spinning...\n");
    while (1)
        ;
    return 0;
}
