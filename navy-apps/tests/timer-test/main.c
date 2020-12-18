
#include <stdio.h>
#include <sys/time.h>
#include <NDL.h>

int main()
{
    int sec = 1;
    struct timeval now;
    printf("Start\n");
    while (1)
    {
        while (NDL_GetTicks() / 1000 < sec)
            ;
        if (sec == 1)
            printf("This is %d second.\n", sec);
        else
            printf("This is %d seconds.\n", sec);
        sec++;
    }

    return 0;
}
