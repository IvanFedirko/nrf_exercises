#include <zephyr/kernel.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>

/************** LSM6DS3 Register  *******************/

#define LSM6DS3_ACC_GYRO_CTRL1_XL 0X10

#define LSM6DS3_ACC_GYRO_CTRL6_G 0X15

#define LSM6DS3_ACC_GYRO_TAP_CFG1 0X58
#define LSM6DS3_ACC_GYRO_WAKE_UP_THS 0X5B
#define LSM6DS3_ACC_GYRO_WAKE_UP_DUR 0X5C
#define LSM6DS3_ACC_GYRO_MD1_CFG 0X5E

/************ End LSM6DS3 Register  *****************/

#define LED2_NODE DT_ALIAS(led2)
static const struct gpio_dt_spec led_blue = GPIO_DT_SPEC_GET(LED2_NODE, gpios);

#define I2C0_NODE DT_NODELABEL(lsm6ds3tr_c)
static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C0_NODE);

#define SW1_NODE DT_ALIAS(sw1)
static const struct gpio_dt_spec wake_up_trigger = GPIO_DT_SPEC_GET(SW1_NODE, gpios);

static struct gpio_callback trigger_cb_data;

void WriteRegister(u_int8_t address, u_int8_t data);

void on_trigger(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
        printk("Wake Up!\n\r");
        gpio_pin_set_dt(&led_blue, true);
        k_msleep(2000);
        gpio_pin_set_dt(&led_blue, false);
}

void lsm6ds_init()
{
        WriteRegister(0x10, 0x0);
        WriteRegister(0x11, 0x0);
        WriteRegister(0x13, 0x0);
        k_msleep(4);

        WriteRegister(LSM6DS3_ACC_GYRO_WAKE_UP_DUR, 0x00); // No duration
        WriteRegister(LSM6DS3_ACC_GYRO_WAKE_UP_THS, 0x02); // Set wake-up threshold
        WriteRegister(LSM6DS3_ACC_GYRO_TAP_CFG1, 0x80);    // Enable interrupts and apply slope filter; latch mode disabled
        WriteRegister(LSM6DS3_ACC_GYRO_CTRL1_XL, 0x70);    // Turn on the accelerometer
                                                           // ODR_XL = 833 Hz, FS_XL = ±2 g
        k_msleep(4);                                       // Delay time per application note
        WriteRegister(LSM6DS3_ACC_GYRO_CTRL1_XL, 0xB0);    // ODR_XL = 1.6 Hz
        WriteRegister(LSM6DS3_ACC_GYRO_CTRL6_G, 0x10);     // High-performance operating mode disabled for accelerometer
        WriteRegister(LSM6DS3_ACC_GYRO_MD1_CFG, 0x20);     // Wake-up interrupt driven to INT1 pin
        k_msleep(4);
}

int main(void)
{
        int ret;

        if (!device_is_ready(dev_i2c.bus))
        {
                printk("I2C bus %s is not ready!\n\r", dev_i2c.bus->name);
                return 0;
        }

        if (!device_is_ready(wake_up_trigger.port))
        {
                return -1;
        }

        if (!gpio_is_ready_dt(&led_blue))
        {
                return -1;
        }

        ret = gpio_pin_configure_dt(&led_blue, GPIO_OUTPUT_ACTIVE);
        if (ret < 0)
        {
                return -1;
        }

        ret = gpio_pin_configure_dt(&wake_up_trigger, GPIO_INPUT);
        if (ret < 0)
        {
                return -1;
        }

        lsm6ds_init();

        gpio_pin_interrupt_configure_dt(&wake_up_trigger, GPIO_INT_EDGE_TO_ACTIVE || GPIO_INT_EDGE_TO_INACTIVE);

        gpio_init_callback(&trigger_cb_data, on_trigger, BIT(wake_up_trigger.pin));

        gpio_add_callback(wake_up_trigger.port, &trigger_cb_data);

        printk("Deep Sleep in 3sec!\n\r");

        gpio_pin_set_dt(&led_blue, false);

        k_msleep(3000);
        NRF_POWER->SYSTEMOFF = 1;

        return 0;
}

void WriteRegister(u_int8_t address, u_int8_t data)
{

        uint8_t config[2] = {address, data};
        int ret = i2c_write_dt(&dev_i2c, config, sizeof(config));
        if (ret != 0)
        {
                printk("Failed to write to I2C device address %x at reg. %x \n\r", dev_i2c.addr, config[0]);
        }
        else
                printk("Ok to write to I2C device address %x at reg. %x \n\r", dev_i2c.addr, config[0]);
}
