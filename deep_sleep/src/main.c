#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(deep_sleep, LOG_LEVEL_DBG);

#define LED0_NODE DT_ALIAS(led1)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
        int ret;
        // Check if port is ready
        if (!device_is_ready(led.port))
        {
                return -1;
        }

        ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
        if (ret < 0)
        {
                return -1;
        }

        gpio_pin_set_dt(&led, 1); //LED KEEP ON IN DEEP SLEEP
        k_msleep(200);
        gpio_pin_set_dt(&led, 0);

        LOG_WRN("GO TO DEEP SLEEP in 3 sec");
        k_msleep(3000);

        // Execution should not go beyond this
        NRF_POWER->SYSTEMOFF = 1;

        while (1)
        {
                LOG_ERR("No sleep");
        }

        return 0;
}
