#ifndef SSD1306_H_
#define SSD1306_H_

#include <stdint.h>
#include <stddef.h>

typedef struct {
    void* data;
    void (*init)(void* data);
    void (*finalize)(void* data);
    void (*send_byte)(void* data, uint8_t reg, uint8_t byte);
    void (*send_bytes)(void* data, uint8_t reg, uint8_t const* bytes, size_t sz);
} I2CFunctions;

void ssd1306_init(I2CFunctions i2c_functions);
void ssd1306_close();

#endif
