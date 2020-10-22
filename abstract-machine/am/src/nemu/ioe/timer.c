#include <am.h>
#include <nemu.h>

uint32_t BOOT_TIME = 0;
void __am_timer_init()
{
    BOOT_TIME = inl(RTC_ADDR);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime)
{
    uptime->us = inl(RTC_ADDR) - BOOT_TIME;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc)
{
    rtc->second = 0;
    rtc->minute = 0;
    rtc->hour = 0;
    rtc->day = 0;
    rtc->month = 0;
    rtc->year = 1900;
}
