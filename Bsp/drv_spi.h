#ifndef __DRV_SPI_H__
#define __DRV_SPI_H__

#include "drv_gpio.h"
#include "board.h"

#define sFLASH_SPI                       SPI2
#define sFLASH_SPI_CLK                   RCC_APB1Periph_SPI2

#define sFLASH_SPI_SCK_PIN               GPIO_Pin_13                  /* PA.05 */
#define sFLASH_SPI_SCK_GPIO_PORT         GPIOB                       /* GPIOA */
#define sFLASH_SPI_SCK_GPIO_CLK          RCC_APB2Periph_GPIOB

#define sFLASH_SPI_MISO_PIN              GPIO_Pin_14                  /* PA.06 */
#define sFLASH_SPI_MISO_GPIO_PORT        GPIOB                       /* GPIOA */
#define sFLASH_SPI_MISO_GPIO_CLK         RCC_APB2Periph_GPIOB

#define sFLASH_SPI_MOSI_PIN              GPIO_Pin_15                  /* PA.07 */
#define sFLASH_SPI_MOSI_GPIO_PORT        GPIOB                       /* GPIOA */
#define sFLASH_SPI_MOSI_GPIO_CLK         RCC_APB2Periph_GPIOB

#define sFLASH_CS_PIN                    GPIO_Pin_12                  /* PA.02 */
#define sFLASH_CS_GPIO_PORT              GPIOB                       /* GPIOA */
#define sFLASH_CS_GPIO_CLK               RCC_APB2Periph_GPIOB

#define sFLASH_CS_LOW()       GPIO_ResetBits(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)
#define sFLASH_CS_HIGH()      GPIO_SetBits(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)

#endif /* end of include guard: __DRV_SPI_H__ */'
