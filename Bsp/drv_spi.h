#ifndef __DRV_SPI_H__
#define __DRV_SPI_H__

#include "drv_gpio.h"
#include "board.h"

struct rt_spi_message
{
    const void *send_buf;
    void *recv_buf;
    uint32_t length;
    struct rt_spi_message *next;

    unsigned cs_take    : 1;
    unsigned cs_release : 1;
};

#endif /* end of include guard: __DRV_SPI_H__ */'
