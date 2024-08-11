#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(wake_up_by_gpio, LOG_LEVEL_DBG);

#define LED0_NODE DT_ALIAS(led1)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

#define SW0_NODE DT_ALIAS(sw0)
static const struct gpio_dt_spec trigger = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

void pin_triggered(const struct device *dev, struct gpio_callback *cb,
                   uint32_t pins)
{
        LOG_WRN("GPIO triggered!");
        LOG_INF("WAKE UP");
        gpio_pin_toggle_dt(&led);
}

// Create struct for callback
static struct gpio_callback trigger_cb_data;

int main(void)
{
        int ret;
        // Check if port is ready
        if (!device_is_ready(trigger.port))
        {
                return -1;
        }

        if (!device_is_ready(led.port))
        {
                return -1;
        }

        // Configure interrupt fo GPIO

        ret = gpio_pin_configure_dt(&trigger, GPIO_INPUT);
        if (ret < 0)
        {
                return -1;
        }

        ret = gpio_pin_interrupt_configure_dt(&trigger, GPIO_INT_EDGE_TO_ACTIVE || GPIO_INT_EDGE_TO_INACTIVE);
        if (ret < 0)
        {
                return -1;
        }

        // Initialization callback
        gpio_init_callback(&trigger_cb_data, pin_triggered, BIT(trigger.pin));

        // Add callback
        gpio_add_callback(trigger.port, &trigger_cb_data);

        ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
        if (ret < 0)
        {
                return -1;
        }

        k_msleep(3000);
        gpio_pin_set_dt(&led, 1); // LED KEEP ON IN DEEP SLEEP
        k_msleep(200);
        gpio_pin_set_dt(&led, 0); // Turn off led

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
