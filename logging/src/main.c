#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(logging, LOG_LEVEL_DBG);

int main(void)
{
        int i = 0;
        while (1)
        {
                LOG_INF("A info log message");
                LOG_INF("Counter %d", i++);
                LOG_DBG("A log message in debug level");
                LOG_WRN("A log message in warning level!");
                LOG_ERR("A log message in Error level!");

                k_msleep(1000);
        }

        return 0;
}
