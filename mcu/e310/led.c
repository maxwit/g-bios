#include <types.h>
#include <init.h>

// TODO: move to reg.h
#define E310_GPIO_BASE 0x10012000

struct e310_gpio {
    __u32 input_val;
    __u32 input_en;
    __u32 output_en;
    __u32 output_val;
};

static int __init redv_led_on()
{
    volatile struct e310_gpio *gpio = (struct e310_gpio *)E310_GPIO_BASE;
    
    gpio->output_en  |= 1 << CONFIG_GPIO_LED;
    gpio->output_val |= 1 << CONFIG_GPIO_LED;

    return 0;
}

arch_init(redv_led_on);
