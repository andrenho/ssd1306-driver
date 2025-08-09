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

void        ssd1306_init(I2CFunctions i2c_functions, size_t lines);
void        ssd1306_close();
void        ssd1306_clear_screen();
SSD_Buffer* ssd1306_create_buffer();
void        ssd1306_render_buffer(SSD_Buffer const* buffer);

void        buffer_free(SSD_Buffer* bf);
void        buffer_clear(SSD_Buffer* bf);
void        buffer_set_pixel(SSD_Buffer* bf, int x, int y, bool color);

#endif
