#include <zephyr/kernel.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>

#include <zephyr/drivers/gpio.h>

#include <zephyr/logging/log.h>

#define SW0_NODE DT_ALIAS(sw0)
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

LOG_MODULE_REGISTER(button_led_callback, LOG_LEVEL_DBG);


void button_pressed(const struct device *dev, struct gpio_callback *cb,
                    uint32_t pins)
{

        // Code in ISR runs at a high priority, therefore, it should be written with timing in mind.
        // Too lengthy or too complex tasks should not be performed by an ISR, they should be deferred to a thread

        bool val = gpio_pin_get_dt(&button);
        gpio_pin_set_dt(&led, val);
        LOG_WRN("Button pressed %d", val);
}

// Create struct for callback
static struct gpio_callback button_cb_data;

int main(void)
{
        int ret;
        LOG_INF("Press/Release button to turn on/off LED");

        // Check if port is ready
        if (!device_is_ready(led.port))
        {
                return -1;
        }
        // Check if port is ready
        if (!device_is_ready(button.port))
        {
                return -1;
        }
        // Set LED0 (Red Led) to Hi
        ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
        if (ret < 0)
        {
                return -1;
        }

        // Config button to INPUT
        ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
        if (ret < 0)
        {
                return -1;
        }

        bool val = gpio_pin_get_dt(&button);
        gpio_pin_set_dt(&led, val);

        // Configure interrupt fo GPIO,
        ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE || GPIO_INT_EDGE_TO_INACTIVE );
        if (ret < 0)
        {
                return -1;
        }

        // Initialization callback
        gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));

        // Add callback
        gpio_add_callback(button.port, &button_cb_data);

        while (1)
        {
                k_msleep(100000000);
        }

        return 0;
}
