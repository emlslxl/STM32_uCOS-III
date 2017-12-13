#include "bsp_rtc.h"

static int month_days[12] = {   31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

#define FEBRUARY        2
#define STARTOFTIME     1970
#define SECDAY          86400L           /*  一天有多少s */
#define SECYR           (SECDAY * 365)
#define leapyear(year)      ((year) % 4 == 0)
#define days_in_year(a)     (leapyear(a) ? 366 : 365)
#define days_in_month(a)    (month_days[(a) - 1])


struct rtc_time tm;


/*
************************************************************************************************************************
*
* Description: 初始化配置RTC
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    :
************************************************************************************************************************
*/

void RTC_Configuration(void)
{
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    BKP_DeInit();

    /* Enable LSE */
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC registers synchronization
     * 因为RTC时钟是低速的，内环时钟是高速的，所以要同步
     */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Set RTC prescaler: set RTC period to 1sec */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    RTC_SetCounter(60);
}


void to_tm(u32 tim, struct rtc_time *tm)
{
    register u32    i;
    register long   hms, day;

    day = tim / SECDAY;         /* 有多少天 */
    hms = tim % SECDAY;         /* 今天的时间，单位s */

    /* Hours, minutes, seconds are easy */
    tm->tm_hour = hms / 3600;
    tm->tm_min = (hms % 3600) / 60;
    tm->tm_sec = (hms % 3600) % 60;

    /* Number of years in days */ /*算出当前年份，起始的计数年份为1970年*/
    for (i = STARTOFTIME; day >= days_in_year(i); i++)
    {
        day -= days_in_year(i);
    }
    tm->tm_year = i;

    /* Number of months in days left */ /*计算当前的月份*/
    if (leapyear(tm->tm_year))
    {
        days_in_month(FEBRUARY) = 29;
    }
    for (i = 1; day >= days_in_month(i); i++)
    {
        day -= days_in_month(i);
    }
    days_in_month(FEBRUARY) = 28;
    tm->tm_mon = i;

    /* Days are what is left over (+1) from all that. *//*计算当前日期*/
    tm->tm_mday = day + 1;

    /*
     * Determine the day of week
     */


}


void Time_Display(void)
{
    to_tm(RTC_GetCounter(), &tm);/*把定时器的值转换为北京时间*/

    /* 输出时间戳，公历时间 */
    printf(" 开机时间:  %0.2d:%0.2d:%0.2d\r\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}








