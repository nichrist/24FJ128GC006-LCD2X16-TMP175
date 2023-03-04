/* 
 * File:   LCD16C.c
 * Author: nichrist
 *MIT License

Copyright (c) [2020] [Nick Christodoulou] [ nichrist@sch.gr ]

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
 * Created on June 15, 2020, 8:26 AM
 */
#include "LCD16C.h"

/*  The LCD can be used with 3.3V power supply but character intensity
 will be low. 
 Configure in MCC Pin Module some free GPIOs
 * 
 *LCD Screen___|_________PIC BOARD________________________________________
 *       D4 <---- Signal from PIC RD0(GPIO output) LCD_D4 as custom name
 *       D5 <---- Signal from PIC RD1(GPIO output) LCD_D5 as custom name
 *       D6 <---- Signal from PIC RD2(GPIO output) LCD_D6 as custom name
 *       D7 <---- Signal from PIC RD3(GPIO output) LCD_D7 as custom name
 *       E  <---- Signal from PIC RD5(GPIO output) LCD_EN as custom name
 *       RW <---- GROUND (only write)
 *       RS <---- Signal from PIC RD4(GPIO output) LCD_RS as custom name
 *       VO <---- 0-5V (middle of pot tied to 5V and GND) Intensity adjustment
 *      VDD <---- 5V power supply
 *      VSS <---- GROUND
 *  
 * The following includes require from mcc to use
 * 1. Delay module
 * 2. Pin manager module, 6 GPIO pins (5V tolerant e.g. RD0-RD5) will be 
 *     needed with the following custom names:
 *     LCD_D4, LCD_D5, LCD_D6, LCD_D7, LCD_RS, LCD_EN 
 * Aditionaly, "mcc_generated_files" project folder and the one that
 * TMP175.h is stored, must be added to project properties->
 * ->XC16->XC16gcc->Preprocessing and messages->C include dirs
 * 
 * The LCD can be used with 3.3V power supply but character intensity
 * will be low.   
 */
#include "delay.h"
#include "pin_manager.h"

LCD2X16 myLCD2X16 = {// Not compulsory detailed initialization
  //  ._displayfunction = 0x00,
  //  .displayControl = 0x00
};

// Low Level Functions call only inside this file (private)

void pulseEnable(void)
{
  LCD_EN_SetLow();
  DELAY_microseconds(1);
  LCD_EN_SetHigh();
  DELAY_microseconds(1); // enable pulse must be >450ns
  LCD_EN_SetLow();
  DELAY_microseconds(100); // commands need > 37us to settle
}

void write4bits(uint8_t value)
{
  int i;
  int temp;
  for (i = 0; i < 4; i++)
    {
      temp = (value >> i) & 0x01;
      switch(i)
        {
        case 0:
          if (temp == 0)
            LCD_D4_SetLow();
          else
            LCD_D4_SetHigh();
          break;
        case 1:
          if (temp == 0)
            LCD_D5_SetLow();
          else
            LCD_D5_SetHigh();
          break;
        case 2:
          if (temp == 0)
            LCD_D6_SetLow();
          else
            LCD_D6_SetHigh();
          break;
        case 3:
          if (temp == 0)
            LCD_D7_SetLow();
          else
            LCD_D7_SetHigh();
          break;
        }
    }
  pulseEnable();
}

void send(uint8_t value, uint8_t mode) // write either command or data
{
  if (mode == 0) // Command
    LCD_RS_SetLow();
  else // Data
    LCD_RS_SetHigh();
  write4bits(value >> 4);
  write4bits(value);
}

// Mid level Functions, for sending data/commads

void command(uint8_t value)
{
  send(value, 0);
}

int data(uint8_t value)
{
  send(value, 1);
  return 1; // assume sucess
}

// User API calls

void InitLCD2X16(LCD2X16* this)
{
  this->displayControl = 0x00;
  this->_displayfunction = 0x00;
  this->_numlines = 0x01;

  write4bits(0x00);
  LCD_RS_SetLow();
  LCD_EN_SetLow();
  DELAY_milliseconds(20);
  write4bits(0x03); //  Function set (Interface is still 8 bits long)
  DELAY_milliseconds(5);
  write4bits(0x03); //  Function set (Interface is still 8 bits long)
  DELAY_microseconds(1);
  write4bits(0x03); //  Function set (Interface is still 8 bits long)
  DELAY_microseconds(125); // Important!!!
  write4bits(0x02);
  DELAY_microseconds(125); // Important!!!
  command(0x28); //Set Data length = 4-bits, Display Lines=2 and Font 5X8 dots
  DELAY_microseconds(40); // ENTRERING 4 mode operation!!!!
  command(0x08); // Display Off
  DELAY_microseconds(40);
  command(0x01); // Clears display
  DELAY_microseconds(40);
  command(0x06); // Direction=LEFT, Auto-increment=ON
  DELAY_milliseconds(125);
  command(0x0F); // Display=ON, Cursor=ON , Blinky=ON (1_D_C_B) 
  DELAY_milliseconds(1);
  command(0x01); // Clear Display 
  DELAY_milliseconds(125);
}

void LcdClear(void)
{
  command(LCD_CLEARDISPLAY); // Clear display, set cursor position to zero
  DELAY_microseconds(2000); // this command takes a long time!
}

void LcdHome(void)
{
  command(LCD_RETURNHOME); // set cursor position to zero
  DELAY_microseconds(2000); // this command takes a long time!
}

void LcdSetCursorXY(unsigned char x, unsigned char y)
{
  unsigned char address = 0;
  switch(y)
    {
    case 1: address = 0x80; // 0x00
      break;
    case 2: address = 0xc0; // 0x40
      break;
    }
  address += x - 1;
  command(address);
}

void LcdWriteChar(char char_value)
{
  static uint8_t lcd_line = 1;
  switch(char_value)
    {
    case '\f':
      command(0x01); // Clears display and set DDRAM Address to 0
      lcd_line = 1;
      break;
    case '\n':
      LcdSetCursorXY(1, ++lcd_line); // Send cursor to the beginning of next line        
      break;
    case '\b':
      command(0x10); // Set display off DDRAM untouched
      break; // Sets CGRAM address.CGRAM data is sent and received after this setting
    default:
      data(char_value); // Send char to display
      break;
    }
}

void LcdWriteString(const char *StrC)
{
  while(*StrC)
    LcdWriteChar(*StrC++);
}

void LcdCreateChar(uint8_t location, uint8_t charmap[])
{
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i = 0; i < 8; i++)
    {
      data(charmap[i]);
    }
}



// Turns the underline cursor on/off
void LcdNoCursor(LCD2X16* this) 
{
  this->displayControl &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | this->displayControl);
}
void LcdCursor(LCD2X16* this) 
{
  this->displayControl |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | this->displayControl);
}