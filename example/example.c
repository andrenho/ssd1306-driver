#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#include "../ssd1306.h"
#include "../toshiba_font.h"

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

    buffer_draw_pixel(bf, 127, 31, true);
    buffer_draw_string(bf, &toshiba_font, 0, 2, "Hello world!");
    buffer_draw_string(bf, &toshiba_font, 8, 18, "Hello world!");
    // buffer_fill_rect(bf, 10, 10, 30, 15, false);
    ssd1306_render_buffer(bf);

    buffer_free(bf);
    ssd1306_close();
}
