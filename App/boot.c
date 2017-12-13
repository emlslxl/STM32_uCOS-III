#include "boot.h"

void Task_BootHandler(void *p_arg)
{
    OS_ERR err;
    (void)p_arg;

    while (1)
    {
        rt_led_blue_on();
        OSTimeDly(100, OS_OPT_TIME_DLY, &err);
        rt_led_blue_off();
        OSTimeDly(100, OS_OPT_TIME_DLY, &err);
    }
}

/* -------------------------------------end of file------------------------------------ */
