#include "ssd1306.h"

#include <stdlib.h>
#include <string.h>

#define CMD  0x00
#define DATA 0xc0
#define DCNT 0x40
#define L (uint8_t const[])

static I2CFunctions f;
static size_t       lines;
static uint8_t*     pixels;

void ssd1306_init(I2CFunctions i2c_functions, size_t lines_)
{
    f = i2c_functions;
    lines = lines_;

    if (f.init)
        f.init(f.data);

    f.send_bytes(f.data, L{ CMD, 0xae }, 2);        // set display off
    f.send_bytes(f.data, L{ CMD, 0xd5, 0x80 }, 1);  // set clock
    f.send_bytes(f.data, L{ CMD, 0xa8, 0x1f }, 1);  // set multiplex ratio
    f.send_bytes(f.data, L{ CMD, 0xd3, 0x00 }, 1);  // set display offset
    f.send_bytes(f.data, L{ CMD, 0x40 }, 2);        // set display start line
    f.send_bytes(f.data, L{ CMD, 0x8d, 0x14 }, 3);  // set charge pump
    f.send_bytes(f.data, L{ CMD, 0x20, 0x00 }, 3);  // horizontal addressing mode
    f.send_bytes(f.data, L{ CMD, 0xa1 }, 2);        // segment remap
    f.send_bytes(f.data, L{ CMD, 0xc8 }, 2);        // set COM output scan direction
    f.send_bytes(f.data, L{ CMD, 0xda, 0x02 }, 3);  // set COM pins hardware config
    f.send_bytes(f.data, L{ CMD, 0x81, 0x8f }, 3);  // set contrast
    f.send_bytes(f.data, L{ CMD, 0xd9, 0xf1 }, 3);  // set precharge period
    f.send_bytes(f.data, L{ CMD, 0xdb, 0x40 }, 3);  // VCOMH deselect level
    f.send_bytes(f.data, L{ CMD, 0xa4 }, 2);        // entire display on
    f.send_bytes(f.data, L{ CMD, 0xa6 }, 2);        // normal/inverse display
    f.send_bytes(f.data, L{ CMD, 0xaf }, 2);        // turn display on

    pixels = calloc(lines, 128 / 8);
}


void ssd1306_close()
{
    if (f.finalize)
        f.finalize(f.data);
}

uint8_t* ssd1306_pixels()
{
    return pixels;
}

void ssd1306_render()
{
    f.send_bytes(f.data, L{ CMD, 0x21, 0x00, 127 }, 4);    // column address: 0~127
    f.send_bytes(f.data, L{ CMD, 0x22, 0x00, 3 }, 4);      // page address: 0~3

    uint8_t data[17];

    // first line
    data[0] = DATA;
    memcpy(&data[1], pixels, 16);
    f.send_bytes(f.data, pixels, 17);

    // remaining lines
    data[0] = DCNT;
    f.send_bytes(f.data, pixels, 17);
    for (size_t i = 1; i < lines; ++i) {
        memcpy(&data[1], &pixels[i * 16], 16);
        f.send_bytes(f.data, data, 17);
    }
}

#undef L
