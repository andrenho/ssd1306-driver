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

static void i2c_send_bytes(void* data, uint8_t const* bytes, size_t sz)
{
    (void) data;
    i2cWriteDevice(H, (char *) bytes, sz);

    /*
    for (size_t i = 0; i < sz; ++i)
        printf("%02X ", bytes[i]);
    printf("\n");
    */
}

int main()
{
    ssd1306_init((I2CFunctions) {
        .data = (void *) 1,
        .init = i2c_init,
        .send_bytes = i2c_send_bytes,
        .finalize = i2c_finalize,
    }, 32);

    SSD_Buffer* bf = ssd1306_create_buffer();

    for (size_t i = 0; i < 512; ++i)
        bf->pixels[i] = 0xff;
    
    ssd1306_render_buffer(bf);

    buffer_free(bf);
    ssd1306_close();
}
