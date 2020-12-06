#include <stdio.h>
#include <NDL.h>

int main()
{
    FILE *fp = fopen("/dev/events", "r");
    NDL_Init(0);
    while (1)
    {
        char buf[64];
        if (NDL_PollEvent(buf, sizeof(buf)))
        {
            printf("receive event: %s\n", buf);
        }
    }
    fclose(fp);
    return 0;
}
