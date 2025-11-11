#ifndef PI_HOME_SENSORS_DISPLAY_H
#define PI_HOME_SENSORS_DISPLAY_H

/*
    1602 (16x2) LCD module with an HD44780 controller,
    connected through a PCF8574 I²C backpack.
*/
#include <stdint.h>
#include "i2c.h"

typedef enum
{
    /* Mode - Sending data */
    DISPLAY_MODE_CMD = 0,

    /* Mode - Sending data */
    DISPLAY_MODE_DATA = 1
} DisplayMode;

typedef enum
{
    DISPLAY_WRITE = 0,
    DISPLAY_READ = 1
} DisplayRW;

typedef enum
{
    /* LCD RAM address for the 1st line */
    DISPLAY_LINE_1 = 0x80,

    /* LCD RAM address for the 2nd line */
    DISPLAY_LINE_2 = 0xC0,
} DisplayLine;

typedef enum
{
    /* Backlight On */
    DISPLAY_BACKLIGHT_ON = 0x08,

    /* Backlight Off */
    DISPLAY_BACKLIGHT_OFF = 0x00
} DisplayBacklight;

typedef union
{
    /* Clears entire display and sets DDRAM address 0 in address counter. */
    struct
    {
        uint8_t clear : 1;
        uint8_t : 7;
    } clear_display;

    /* Sets DDRAM address 0 in address counter.
       Also returns display from being shifted to original position.
       DDRAM contents remain unchanged */
    struct
    {
        uint8_t : 1;
        uint8_t return_home : 1;
        uint8_t : 6;

    } return_home;

    /* Sets cursor move direction and specifies display shift.
       These operations are performed during data write and read.*/
    struct
    {
        uint8_t display_shift : 1;
        enum
        {
            DISPLAY_ENTRY_INCREMENT = 0,
            DISPLAY_ENTRY_DECREMENT = 1
        } entry_increment : 1;
        uint8_t mode_set : 1;
        uint8_t : 5;
    } entry_mode_set;

    /*  Sets entire display on/off, cursor on/off,
        and blinking of cursor position character. */
    struct
    {
        enum
        {
            DISPLAY_OFF = 0,
            DISPLAY_ON = 1
        } display_on : 1;
        enum
        {
            CURSOR_OFF = 0,
            CURSOR_ON = 1
        } cursor_on : 1;
        enum
        {
            BLINK_OFF = 0,
            BLINK_ON = 1
        } blink_on : 1;
        uint8_t display_control : 1;
        uint8_t : 4;
    } display_control;

    /* Moves cursor and shifts display without changing DDRAM contents.*/
    struct
    {
        uint8_t : 2;
        enum
        {
            SHIFT_TO_LEFT = 0,
            SHIFT_TO_RIGHT = 1
        } shift_dir : 1;
        enum
        {
            CURSOR_MOVE = 0,
            DISPLAY_SHIFT = 1
        } shift_type : 1;
        uint8_t shift : 1;
        uint8_t : 4;
    } cursor_or_display_shift;

    /* Sets interface data length, number of display lines,
       and character font. */
    struct
    {
        uint8_t : 2;
        enum
        {
            DISPLAY_5x8_DOTS = 0,
            DISPLAY_5x10_DOTS = 1,
        } resolution : 1;
        enum
        {
            DISPLAY_1_LINE = 0,
            DISPLAY_2_LINES = 1,
        } lines : 1;
        enum
        {
            INTERFACE_4BIT = 0,
            INTERFACE_8BIT = 1,
        } interface_data_length : 1;
        uint8_t set : 1;
        uint8_t : 2;
    } function_set;

    uint8_t byte;
} DisplayInstructions;

/* PCF8574 backpack I2C byte bitfield. Each bit controls one pin on the HD44780. */
typedef union
{
    struct
    {
        /* P0 (0 = command, 1 = data) */
        uint8_t rs : 1;

        /* P1 (0 = write, 1 = read) */
        uint8_t rw : 1;

        /* P2 (1 = latch data) */
        uint8_t enable : 1;

        /* P3 (1 = backlight ON) */
        uint8_t backlight : 1;

        /* D4..D7  (high nibble of the command or data) */
        uint8_t data : 4;
    } pins;
    uint8_t byte;
} DisplayI2CByte;

typedef struct
{
    struct I2cBus *i2c_bus;
    uint8_t i2c_addr;
} Display;

void display_init(struct I2cBus *i2c_bus);
void display_string(const char *message, int line);

#endif /* PI_HOME_SENSORS_DISPLAY_H */