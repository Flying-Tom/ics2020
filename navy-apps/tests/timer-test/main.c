#include <stdio.h>
#include <NDL.h>
#include <sys/time.h>

int main()
{
    int sec = 1;
    struct timeval now;
    while (1)
    {
        while (NDL_GetTicks() / 1000000 < sec)
            printf("%llu\n", NDL_GetTicks());
        if (sec == 1)
            printf("This is %d second.\n", sec);
        else
            printf("This is %d seconds.\n", sec);
        sec++;
    }

    return 0;
}
