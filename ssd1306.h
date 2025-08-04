#ifndef SSD1306_H_
#define SSD1306_H_

typedef struct {

} I2CFunctions;

void ssd1306_init(I2CFunctions i2c_functions);
void ssd1306_close();

#endif
