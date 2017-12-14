#include "drv_gpio.h"

void rt_gpio_set_mode(GPIO_TypeDef *port, uint16_t pin,
                      GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_InitStructure.GPIO_Speed = speed;
    GPIO_Init(port, &GPIO_InitStructure);
}

int bsp_gpio_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);

    //led
    rt_gpio_set_mode(PORT_GPIO_LED_RED, PIN_GPIO_LED_RED | PIN_GPIO_LED_GREEN,
                     GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
    rt_gpio_set_mode(PORT_GPIO_LED_BLUE, PIN_GPIO_LED_BLUE | PIN_GPIO_LED_YELLOW,
                     GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
    rt_led_blue_off();
    rt_led_green_off();
    rt_led_red_off();

    // 外围芯片电源控制
    rt_gpio_set_mode(PORT_GPIO_COMMON_POWER, PIN_GPIO_COMMON_POWER,
                     GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
    rt_power_common_on();

    //GPS电源
    rt_gpio_set_mode(PORT_GPIO_GPS_POWER, PIN_GPIO_GPS_POWER,
                     GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
    rt_power_gps_off();

    //spi_nand/lis3dh电源
    rt_gpio_set_mode(PORT_GPIO_SD_POWER, PIN_GPIO_SD_POWER,
                     GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
    rt_power_sd_on();

    //GPRS电源
    rt_gpio_set_mode(PORT_GPIO_GPRS_POWER, PIN_GPIO_GPRS_POWER,
                     GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
    rt_power_gprs_off();

    //BLE
    rt_gpio_set_mode(PORT_GPIO_BLE_POWER, PIN_GPIO_BLE_POWER,
                     GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
    rt_power_ble_off();


    //SPI HOLD/WP
    rt_gpio_set_mode(PORT_GPIO_SPI2_HOLD, PIN_GPIO_SPI2_HOLD,
                     GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
    GPIO_SetBits(PORT_GPIO_SPI2_HOLD, PIN_GPIO_SPI2_HOLD);

    rt_gpio_set_mode(PORT_GPIO_SPI2_WP, PIN_GPIO_SPI2_WP,
                     GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
    GPIO_SetBits(PORT_GPIO_SPI2_WP, PIN_GPIO_SPI2_WP);

    return 0;
}

