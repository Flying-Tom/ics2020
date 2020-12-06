#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

static uint64_t get_time()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    uint32_t seconds = now.tv_sec;
    uint32_t useconds = now.tv_usec;
    return seconds * 1000 + (useconds + 500) / 1000;
}

int main()
{
    uint64_t boot_time = get_time();
    while (1)
    {
        printf("%u\n", get_time());
    }

    return 0;
}
