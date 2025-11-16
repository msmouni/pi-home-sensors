/*
    1602 (16x2) LCD module with an HD44780 controller,
    connected through a PCF8574 I²C backpack.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "display.h"

#define PCF8574_I2C_ADDR 0x27

static Display display = {.i2c_bus = NULL, .i2c_addr = PCF8574_I2C_ADDR};

// #define LCD_CHR 1 // Mode - Sending data
// #define LCD_CMD 0 // Mode - Sending command

// #define LCD_LINE_1 0x80 // LCD RAM address for the 1st line
// #define LCD_LINE_2 0xC0 // LCD RAM address for the 2nd line
// #define LCD_LINE_3 0x94 // LCD RAM address for the 3rd line
// #define LCD_LINE_4 0xD4 // LCD RAM address for the 4th line

// #define LCD_BACKLIGHT 0x08 // On
// // #define LCD_BACKLIGHT   0x00  // Off

// #define ENABLE 0b00000100 // Enable bit

#define E_PULSE 500
#define E_DELAY 500

static void display_data_pulse(DisplayI2CByte byte)
{
    byte.pins.enable = 0;
    i2c_write(display.i2c_bus, display.i2c_addr, (const uint8_t *)&byte, sizeof(byte));

    usleep(E_DELAY);

    byte.pins.enable = 1;
    i2c_write(display.i2c_bus, display.i2c_addr, (const uint8_t *)&byte, sizeof(byte));

    usleep(E_PULSE);

    byte.pins.enable = 0;
    i2c_write(display.i2c_bus, display.i2c_addr, (const uint8_t *)&byte, sizeof(byte));

    usleep(E_DELAY);
}

void display_send_data(DisplayI2CByte *display_data, DisplayMode mode)
{
    DisplayI2CByte high_nibble;
    DisplayI2CByte low_nibble;

    high_nibble.pins.rs = mode;
    high_nibble.pins.rw = DISPLAY_WRITE;
    high_nibble.pins.backlight = 1;
    high_nibble.pins.data = (display_data->byte >> 4) & 0x0F;

    low_nibble.pins.rs = mode;
    low_nibble.pins.rw = DISPLAY_WRITE;
    low_nibble.pins.backlight = 1;
    low_nibble.pins.data = display_data->byte & 0x0F;

    // Send high nibble
    display_data_pulse(high_nibble);

    // Send low nibble
    display_data_pulse(low_nibble);
}

void display_init(struct I2cBus *i2c_bus)
{
    display.i2c_bus = i2c_bus;

    DisplayInstructions instr = {.byte = 0};

    instr.function_set.interface_data_length = INTERFACE_4BIT;
    instr.function_set.lines = DISPLAY_2_LINES;
    instr.function_set.resolution = DISPLAY_5x8_DOTS;
    instr.function_set.set = 1;
    display_send_data(&instr, DISPLAY_MODE_CMD);

    instr.byte = 0;
    instr.entry_mode_set.display_shift = 0;
    instr.entry_mode_set.entry_increment = DISPLAY_ENTRY_INCREMENT;
    instr.entry_mode_set.mode_set = 1;
    display_send_data(&instr, DISPLAY_MODE_CMD);

    instr.byte = 0;
    instr.display_control.display_on = DISPLAY_OFF;
    instr.display_control.cursor_on = CURSOR_OFF;
    instr.display_control.blink_on = BLINK_OFF;
    instr.display_control.display_control = 1;
    display_send_data(&instr, DISPLAY_MODE_CMD);

    // 0x33 // 110011 Initialise
    // 0x32 // 110010 Initialise
    // 0x06 // 000110 Cursor move direction
    // 0x0C // 001100 Display On, Cursor Off, Blink Off
    // 0x28 // 101000 Data length, number of lines, font size
    // 0x01 // 000001 Clear display
    usleep(E_DELAY * 1000);
}

void display_string(const char *message, int line)
{
    DisplayI2CByte data_byte = {.byte = 0};

    data_byte.byte = line;
    display_send_data(&data_byte, DISPLAY_MODE_CMD);

    for (int i = 0; i < 16 && message[i] != '\0'; i++)
    {
        data_byte.byte = message[i];
        display_send_data(&data_byte, DISPLAY_MODE_DATA);
    }
}