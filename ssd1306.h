#ifndef SSD1306_H_
#define SSD1306_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    void* data;
    void (*init)(void* data);
    void (*finalize)(void* data);
    void (*send_bytes)(void* data, uint8_t const* bytes, size_t sz);
} I2CFunctions;

typedef struct {
    uint16_t w;
    uint8_t  h;
    uint8_t* pixels;
} SSD_Buffer;

typedef struct {
    uint8_t char_w;
    uint8_t char_h;
    const uint8_t** chars;
} Font;

void        ssd1306_init(I2CFunctions i2c_functions, size_t lines);
void        ssd1306_close();
void        ssd1306_clear_screen();
SSD_Buffer* ssd1306_create_buffer();
void        ssd1306_render_buffer(SSD_Buffer const* buffer);

void        buffer_free(SSD_Buffer* bf);
void        buffer_clear(SSD_Buffer* bf);
void        buffer_fill_rect(SSD_Buffer* bf, uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color);
void        buffer_draw_pixel(SSD_Buffer* bf, int x, int y, bool color);
void        buffer_draw_char(SSD_Buffer* bf, Font const* font, int x, int y, char c);
void        buffer_draw_string(SSD_Buffer* bf, Font const* font, int x, int y, const char* str);

#endif
