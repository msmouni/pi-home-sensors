#ifndef PI_HOME_SENSORS_DISPLAY_H
#define PI_HOME_SENSORS_DISPLAY_H

/*
    1602 (16x2) LCD module with an HD44780 controller,
    connected through a PCF8574 I²C backpack.
*/
#include <stdint.h>
#include "i2c.h"

#define MAX_PRINT_SIZE 128

void display_create(struct I2cBus *i2c_bus);
void display_destroy(void);

/* Print a string on line 0 or 1 */
void display_print(const char *str, uint8_t line);

/* Clear whole display */
void display_clear(void);

#endif /* PI_HOME_SENSORS_DISPLAY_H */