#include "../ssd1306.h"

#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

static int H;  // handle

static void i2c_init(void* data)
{
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Could not initialize pigpio!\n");
        exit(EXIT_FAILURE);
    }

    int bus = (int) data;
    H = i2cOpen(bus, 0x3c, 0);
}

static void i2c_finalize(void* data)
{
    (void) data;
    i2cClose(H);
    gpioTerminate();
}

static void i2c_send_byte(void* data, uint8_t reg, uint8_t byte)
{
    i2cWriteByteData(H, reg, byte);
}

static void i2c_send_bytes(void* data, uint8_t reg, uint8_t const* bytes, size_t sz)
{
    i2cWriteBlockData(H, reg, bytes, sz);
}

int main()
{
    ssd1306_init((I2CFunctions) {
        .data = (void *) 1,
        .init = i2c_init,
        .send_byte = i2c_send_byte,
        .send_bytes = i2c_send_bytes,
        .finalize = i2c_finalize,
    });

    ssd1306_close();
}
