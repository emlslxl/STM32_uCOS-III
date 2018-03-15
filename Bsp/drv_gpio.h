#ifndef __DRV_GPIO_H__
#define __DRV_GPIO_H__

#include "board.h"

//LED
#define PORT_GPIO_LED_RED       GPIOB
#define PORT_GPIO_LED_GREEN     GPIOC
#define PORT_GPIO_LED_BLUE      GPIOD
#define PORT_GPIO_LED_YELLOW    GPIOD
#define PIN_GPIO_LED_RED        GPIO_Pin_7
#define PIN_GPIO_LED_GREEN      GPIO_Pin_7
#define PIN_GPIO_LED_BLUE       GPIO_Pin_3
#define PIN_GPIO_LED_YELLOW     GPIO_Pin_4
#define rt_led_red_on()         GPIO_SetBits(PORT_GPIO_LED_RED, PIN_GPIO_LED_RED)
#define rt_led_red_off()        GPIO_ResetBits(PORT_GPIO_LED_RED, PIN_GPIO_LED_RED)
#define rt_led_green_on()       GPIO_SetBits(PORT_GPIO_LED_GREEN, PIN_GPIO_LED_GREEN)
#define rt_led_green_off()      GPIO_ResetBits(PORT_GPIO_LED_GREEN, PIN_GPIO_LED_GREEN)
#define rt_led_blue_on()        GPIO_SetBits(PORT_GPIO_LED_BLUE, PIN_GPIO_LED_BLUE)
#define rt_led_blue_off()       GPIO_ResetBits(PORT_GPIO_LED_BLUE, PIN_GPIO_LED_BLUE)
#define rt_led_yellow_on()      GPIO_SetBits(PORT_GPIO_LED_YELLOW, PIN_GPIO_LED_YELLOW)
#define rt_led_yellow_off()     GPIO_ResetBits(PORT_GPIO_LED_YELLOW, PIN_GPIO_LED_YELLOW)

//POWER
#define PORT_GPIO_COMMON_POWER  GPIOE
#define PIN_GPIO_COMMON_POWER   GPIO_Pin_1
#define rt_power_common_on()    GPIO_SetBits(PORT_GPIO_COMMON_POWER, PIN_GPIO_COMMON_POWER)
#define rt_power_common_off()   GPIO_ResetBits(PORT_GPIO_COMMON_POWER, PIN_GPIO_COMMON_POWER)

#define PORT_GPIO_GPS_POWER     GPIOC
#define PIN_GPIO_GPS_POWER      GPIO_Pin_10
#define rt_power_gps_on()       GPIO_SetBits(PORT_GPIO_GPS_POWER, PIN_GPIO_GPS_POWER)
#define rt_power_gps_off()      GPIO_ResetBits(PORT_GPIO_GPS_POWER, PIN_GPIO_GPS_POWER)

#define PORT_GPIO_GPRS_POWER     GPIOB
#define PIN_GPIO_GPRS_POWER      GPIO_Pin_9
#define rt_power_gprs_on()       GPIO_SetBits(PORT_GPIO_GPRS_POWER, PIN_GPIO_GPRS_POWER)
#define rt_power_gprs_off()      GPIO_ResetBits(PORT_GPIO_GPRS_POWER, PIN_GPIO_GPRS_POWER)

#define PORT_GPIO_BLE_POWER     GPIOD
#define PIN_GPIO_BLE_POWER      GPIO_Pin_7
#define rt_power_ble_on()       GPIO_SetBits(PORT_GPIO_BLE_POWER, PIN_GPIO_BLE_POWER)
#define rt_power_ble_off()      GPIO_ResetBits(PORT_GPIO_BLE_POWER, PIN_GPIO_BLE_POWER)

#define PORT_GPIO_SD_POWER     GPIOA
#define PIN_GPIO_SD_POWER      GPIO_Pin_8
#define rt_power_sd_on()       GPIO_SetBits(PORT_GPIO_SD_POWER, PIN_GPIO_SD_POWER)
#define rt_power_sd_off()      GPIO_ResetBits(PORT_GPIO_SD_POWER, PIN_GPIO_SD_POWER)

//GPIO_INPUT
#define PORT_GPIO_DIN_ACC     GPIOE
#define PIN_GPIO_DIN_ACC      GPIO_Pin_2
#define rt_gpio_state_acc()   GPIO_ReadInputDataBit(PORT_GPIO_DIN_ACC, PIN_GPIO_DIN_ACC)

#define PORT_GPIO_DIN_MOTO     GPIOE
#define PIN_GPIO_DIN_MOTO      GPIO_Pin_2
#define rt_gpio_state_moto()   GPIO_ReadInputDataBit(PORT_GPIO_DIN_MOTO, PIN_GPIO_DIN_MOTO)

//SPI HOLD/WP
#define PORT_GPIO_SPI2_HOLD     GPIOE
#define PIN_GPIO_SPI2_HOLD      GPIO_Pin_13

#define PORT_GPIO_SPI2_WP       GPIOE
#define PIN_GPIO_SPI2_WP        GPIO_Pin_14

extern void rt_gpio_set_mode(GPIO_TypeDef *port, uint16_t pin,
                             GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed);

#endif /* end of include guard: __DRV_GPIO_H__ */

