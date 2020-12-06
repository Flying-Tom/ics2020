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
        {    
            gettimeofday(&now, NULL);
            printf("%llu\n", now.tv_usec);
        }
        if (sec == 1)
        {
            printf("%d second).\n", sec);
        }
        else
        {
            printf("%d seconds).\n", sec);
        }
        sec++;
    }

    return 0;
}
