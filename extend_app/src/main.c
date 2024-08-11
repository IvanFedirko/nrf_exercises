#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "myfunction.h"

int main(void)
{
        int val = 10;

        while (1)
        {
                for (int i = 0; i < 10; i++)
                {

#ifdef CONFIG_MYFUNCTION

                        printk("Sum: %d\r\n", sum(val, i));
#else
                        printk("MYFUNCTION not enabled\r\n");
                        return;
#endif

                }
                        k_msleep(2000);
        }



        return 0;
}
