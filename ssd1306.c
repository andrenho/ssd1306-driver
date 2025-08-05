#include "ssd1306.h"

#include <stdlib.h>

#define CMD  0x00
#define DATA 0x40
#define L (uint8_t const[])

static I2CFunctions f;

void ssd1306_init(I2CFunctions i2c_functions)
{

    f = i2c_functions;
    if (f.init)
        f.init(f.data);

    f.send_byte(f.data, CMD, 0xae);                 // set display off
    f.send_bytes(f.data, CMD, L{ 0xd5, 0x80 }, 2);  // set clock
    f.send_bytes(f.data, CMD, L{ 0xa8, 0x1f }, 2);  // set multiplex ratio
    f.send_bytes(f.data, CMD, L{ 0xd3, 0x00 }, 2);  // set display offset
    f.send_byte(f.data, CMD, 0x40);                 // set display start line
    f.send_bytes(f.data, CMD, L{ 0x8d, 0x14 }, 2);  // set charge pump
    f.send_bytes(f.data, CMD, L{ 0x20, 0x00 }, 2);  // horizontal addressing mode
    f.send_byte(f.data, CMD, 0xa1);                 // segment remap
    f.send_byte(f.data, CMD, 0xc8);                 // set COM output scan direction
    f.send_bytes(f.data, CMD, L{ 0xda, 0x02 }, 2);  // set COM pins hardware config
    f.send_bytes(f.data, CMD, L{ 0x81, 0x8f }, 2);  // set contrast
    f.send_bytes(f.data, CMD, L{ 0xd9, 0xf1 }, 2);  // set precharge period
    f.send_bytes(f.data, CMD, L{ 0xdb, 0x40 }, 2);  // VCOMH deselect level
    f.send_byte(f.data, CMD, 0xa4);                 // entire display on
    f.send_byte(f.data, CMD, 0xa6);                 // normal/inverse display
    f.send_byte(f.data, CMD, 0xaf);                 // turn display on
}

void ssd1306_close()
{
    if (f.finalize)
        f.finalize(f.data);
}

#undef L
