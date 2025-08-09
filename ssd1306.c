#include "ssd1306.h"

#include <stdlib.h>
#include <string.h>

#define CMD  0x00
#define DATA 0xc0
#define DCNT 0x40
#define L (uint8_t const[])

static I2CFunctions f;
static size_t       lines;

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
    f.send_bytes(f.data, L{ CMD, 0x20, 0x01 }, 3);  // vertical addressing mode
    f.send_bytes(f.data, L{ CMD, 0xa1 }, 2);        // segment remap
    f.send_bytes(f.data, L{ CMD, 0xc8 }, 2);        // set COM output scan direction
    f.send_bytes(f.data, L{ CMD, 0xda, 0x02 }, 3);  // set COM pins hardware config
    f.send_bytes(f.data, L{ CMD, 0x81, 0x8f }, 3);  // set contrast
    f.send_bytes(f.data, L{ CMD, 0xd9, 0xf1 }, 3);  // set precharge period
    f.send_bytes(f.data, L{ CMD, 0xdb, 0x40 }, 3);  // VCOMH deselect level
    f.send_bytes(f.data, L{ CMD, 0xa4 }, 2);        // entire display on
    f.send_bytes(f.data, L{ CMD, 0xa6 }, 2);        // normal/inverse display
    ssd1306_clear_screen();
    f.send_bytes(f.data, L{ CMD, 0xaf }, 2);        // turn display on
}


void ssd1306_close()
{
    if (f.finalize)
        f.finalize(f.data);
}

void ssd1306_clear_screen()
{
    f.send_bytes(f.data, L{ CMD, 0x21, 0x00, 127 }, 4);      // column address: 0~127
    f.send_bytes(f.data, L{ CMD, 0x22, 0x00, lines / 8 - 1 }, 4);  // page address: 0~3

    // draw
    uint8_t data[17] = {0};
    data[0] = DCNT;
    size_t n_bytes = (128 / 8) * lines;
    for (size_t i = 0; i < n_bytes / 16; ++i)
        f.send_bytes(f.data, data, 17);
}

SSD_Buffer* ssd1306_create_buffer()
{
    SSD_Buffer* bf = calloc(1, sizeof(SSD_Buffer));
    bf->w = 128;
    bf->h = lines;
    bf->pixels = calloc(128 / 8, lines);
    return bf;
}

void ssd1306_render_buffer(SSD_Buffer const* bf)
{
    f.send_bytes(f.data, L{ CMD, 0x21, 0x00, bf->w - 1 }, 4);      // column address: 0~127
    f.send_bytes(f.data, L{ CMD, 0x22, 0x00, bf->h / 8 - 1 }, 4);  // page address: 0~3

    // draw
    uint8_t data[17];
    data[0] = DCNT;
    size_t n_bytes = (bf->w / 8) * bf->h;
    for (size_t i = 0; i < n_bytes / 16; ++i) {
        memcpy(&data[1], &bf->pixels[i * 16], 16);
        f.send_bytes(f.data, data, 17);
    }
}

void buffer_free(SSD_Buffer* buffer)
{
    free(buffer->pixels);
    free(buffer);
}

void buffer_clear(SSD_Buffer* bf)
{
    memset(bf->pixels, 0, (bf->w / 8) * bf->h);
}

void buffer_draw_pixel(SSD_Buffer* bf, int x, int y, bool color)
{
    if (x < 0 || x >= bf->w || y < 0 || y >= bf->h)
        return;

    uint16_t n_pages = bf->h / 8;
    uint16_t byte = (y / 8) + (x * n_pages);
    uint8_t  bit = y % 8;

    if (color)
        bf->pixels[byte] = bf->pixels[byte] | (1 << bit);
    else
        bf->pixels[byte] = bf->pixels[byte] & ~(1 << bit);
}

void buffer_draw_char(SSD_Buffer* bf, Font const* font, int x, int y, char c)
{
    uint8_t const* flat = (uint8_t const*) font->chars;
    uint8_t const* fst_line = &flat[c * font->char_h];

    for (size_t row = 0; row < font->char_h; ++row) {
        for (size_t col = 0; col < font->char_w; ++col) {
            bool data = fst_line[row] & (1 << (7 - col));
            if (data)
                buffer_draw_pixel(bf, x + col, y + row, true);
        }
    }
}

void buffer_draw_string(SSD_Buffer* bf, Font const* font, int x, int y, const char* str)
{
    while (*str) {
        buffer_draw_char(bf, font, x, y, *str);
        x += font->char_w;
        ++str;
    }
}

void buffer_fill_rect(SSD_Buffer* bf, uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color)
{
    for (uint8_t xx = x; xx < (x + w); ++xx)
        for (uint8_t yy = y; yy < (y + h); ++yy)
            buffer_draw_pixel(bf, xx, yy, color);
}

#undef L
