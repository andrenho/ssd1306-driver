#include "../ssd1306.h"

#include <stdio.h>
#include <pigpio.h>

int main()
{
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Could not initialize pigpio!\n");
        return 1;
    }

    int h = i2cOpen(1, 0x3c, 0);
    ssd1306_init((I2CFunctions) {});

    ssd1306_close();
    i2cClose(h);

    gpioTerminate();
}
