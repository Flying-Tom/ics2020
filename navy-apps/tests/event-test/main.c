#include <stdio.h>
#include <NDL.h>

int main()
{
    NDL_Init(0);
    int cnt = 0;
    while (1)
    {
        char buf[64];
        printf("%d\n", cnt++);
        if (NDL_PollEvent(buf, sizeof(buf)))
        {
            printf("receive event: %s\n", buf);
        }
    }
    return 0;
}
