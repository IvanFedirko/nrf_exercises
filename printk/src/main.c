#include <zephyr/kernel.h>

//Adding printk header file
#include <zephyr/sys/printk.h>

int main(void)
{
        //Simple message
        printk("Hello, World!\n\r");
        
        //Decimal value
        int32_t val = -10;
        printk("Single decimal '%%d': %d, %%i:  %i\r\n", val, val);
        uint32_t uval = 10;

        //Unsigned decimal
        printk("Unsigned decimal '%%u (uval)': %u, %%u (val):  %u\r\n", uval, val);
        
        //Pointer
        printk("Pointer '%%p (uval)': %p, %%p (val):  %p\r\n", uval, val);
        
        //String
        char msg[6];
        strcpy(msg, "Hello");
        printk("String %%s: %s, strlen: %d, sizeof: %d\r\r", msg, strlen(msg), sizeof(msg));

        return 0;
}

