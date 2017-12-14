#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

#include "bspdef.h"

#include "os_cfg_app.h"
#include "misc.h"
#include "stm32f10x.h"

#include "drv_gpio.h"
#include "drv_spi.h"

#define RT_EOK                          0               /**< There is no error */
#define RT_ERROR                        1               /**< A generic error happens */
#define RT_ETIMEOUT                     2               /**< Timed out */
#define RT_EFULL                        3               /**< The resource is full */
#define RT_EEMPTY                       4               /**< The resource is empty */
#define RT_ENOMEM                       5               /**< No memory */
#define RT_ENOSYS                       6               /**< No system */
#define RT_EBUSY                        7               /**< Busy */
#define RT_EIO                          8               /**< IO error */

extern void BSP_Init(void);
extern void delay_ms(uint32_t delay);

#endif /* __INCLUDES_H__ */



