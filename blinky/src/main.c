#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>


#define SLEEP_TIME_MS 1000

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);

int main(void)
{
	int ret;

	if (!gpio_is_ready_dt(&led0))
	{
		return 0;
	}
	if (!gpio_is_ready_dt(&led1))
	{
		return 0;
	}
	if (!gpio_is_ready_dt(&led2))
	{
		return 0;
	}

	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return 0;
	}

	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return 0;
	}

	ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return 0;
	}

	while (1)
	{
		for (int volatile  i = 0; i < 3; i++)
		{
			if (i == 0)
			{
				ret = gpio_pin_set_dt(&led0, 1);
				if (ret < 0)
				{
					return 0;
				}

				ret = gpio_pin_set_dt(&led1, 0);
				if (ret < 0)
				{
					return 0;
				}

				ret = gpio_pin_set_dt(&led2, 0);
				if (ret < 0)
				{
					return 0;
				}
			}

			if (i == 1)
			{
				ret = gpio_pin_set_dt(&led0, 0);
				if (ret < 0)
				{
					return 0;
				}

				ret = gpio_pin_set_dt(&led1, 1);
				if (ret < 0)
				{
					return 0;
				}

				ret = gpio_pin_set_dt(&led2, 0);
				if (ret < 0)
				{
					return 0;
				}
			}

			if (i == 2)
			{
				ret = gpio_pin_set_dt(&led0, 0);
				if (ret < 0)
				{
					return 0;
				}

				ret = gpio_pin_set_dt(&led1, 0);
				if (ret < 0)
				{
					return 0;
				}

				ret = gpio_pin_set_dt(&led2, 1);
				if (ret < 0)
				{
					return 0;
				}
			}

			k_msleep(SLEEP_TIME_MS);
		}
	}
	return 0;
}
