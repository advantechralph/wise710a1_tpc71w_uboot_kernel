#include "gpio_test.h"
#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_gpio.h>

static int __init gpio_test_init(void){
    printk("%s, %d: gpio 55 value: %d\n", __FUNCTION__, __LINE__, gpio_get_value(55));
    //printk("%s, %d: gpio 55 is requested: %d\n", __FUNCTION__, __LINE__, gpiochip_is_requested(55));
    gpio_direction_output(55, 1);
    gpio_set_value(55, 0); 
    printk("%s, %d: gpio 55 value: %d\n", __FUNCTION__, __LINE__, gpio_get_value(55));
    return 0;
}

static void __exit gpio_test_exit(void){
    printk("%s:%d\n", __FUNCTION__, __LINE__);
}

module_init(gpio_test_init);
module_exit(gpio_test_exit);
MODULE_DESCRIPTION("GPIO Test Driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ralph Wang");

