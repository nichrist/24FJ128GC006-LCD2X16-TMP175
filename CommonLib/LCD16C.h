/* 
 * File:   LCD.h
 * Author: nichrist
 *Copyright (c) [2020] [Nick Christodoulou] [ nichrist@sch.gr ]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 * Created on July 10, 2020, 6:28 PM
 */

#ifndef LCD16C_H
#define	LCD16C_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

    // commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

    // flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

    // flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

    // flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

    // flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

    typedef struct LCD16C {
        uint8_t _displayfunction;
        uint8_t displayControl;
        uint8_t _displaymode;
        uint8_t _initialized;
        uint8_t _numlines;
        uint8_t _row_offsets[4];
        uint8_t lcd_line;
    } LCD2X16;

    /*
     * Object instance: myLCD2X16
     * ----------------------------
     *   LCD2X16 Instance object to be used in initialization
     *   e.g. InitLCD2X16(&myLCD2X16)
     */
    extern LCD2X16 myLCD2X16; // The instance of LCD object to be initialized 
    // and used in application

    // API Functions
    void InitLCD2X16(LCD2X16 * this);
    void LcdClear(void);
    void LcdHome(void);

    /* Function: LcdSetCursorXY
     * ----------------------------
     *   Position the cursor to x(column) y(row/line) 
     *
     *   x: x character position (Possible Values: 1-16)
     *   y: y character position (Possible Values:1-2)
     *   c: ASCI code Character
     * 
     *   returns: void 
     */
    void LcdSetCursorXY(unsigned char x, unsigned char y);
    void LcdWriteChar(char char_value);
    void LcdWriteString(const char *StrC);
    void LcdCreateChar(uint8_t location, uint8_t charmap[]);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD16C_H */

