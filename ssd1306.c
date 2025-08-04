#include "ssd1306.h"

#include <stdlib.h>

static I2CFunctions f;

void ssd1306_init(I2CFunctions i2c_functions)
{
    f = i2c_functions;
}

void ssd1306_close()
{
}
