#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

int main()
{
    int sec = 1;
    struct timeval now;
    while (1)
    {
        while (now.tv_usec / 1000000 < sec)
            gettimeofday(&now, NULL);
        if (sec == 1)
            printf("This is %d second.\n", sec);
        else
            printf("This is %d seconds.\n", sec);
        sec++;
    }

    return 0;
}
