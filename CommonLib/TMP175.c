/* 
 * File:   TMP175.c
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
 * Created on June 15, 2020, 8:26 AM
 */
#include "TMP175.h"
#include <stdio.h>
#include <stdbool.h>

/* 
 *___TMP175_____|____PIC BOARD I2C1 module_______
 *  SDA(PIN1) <---> PIC I2C Data I/O SDA RD9
 *  SCL(PIN2) <---- PIC I2C Clock SCL RD10 
 *   V+(PIN8) <---- 3V3 or 5V power supply
 *  GND(PIN4) <---- GROUND 
 * 
 * The following includes, required from MCC to be used:
 *  1. Delay module
 *  2. I2C1 module configured in master mode, 7bit with acknowledge, 100KHz Clock (tested)
 *  Aditionaly, "mcc_generated_files" project folder and the one that
 *  TMP175.h is stored, must be added to project properties:
 *  properties->XC16->XC16gcc->Preprocessing and messages->C include dirs   
 */
#include "delay.h"  // MCC module
#include "i2c1.h"   // MCC module
#include "pin_manager.h"    // MCC module

TMP175 myTMP175;    // Create the sensor's Object instance

/* Low level private functions */

void I2C_Write(TMP175* self, uint8_t *pdata, uint8_t length)
{
  uint16_t counter = 0;
  I2C1_MESSAGE_STATUS i2cStatus = I2C1_MESSAGE_PENDING;
  self->stat.flag.i2cErrorMessage = i2cStatus;
  I2C1_MasterWrite(pdata,
                   length,
                   self->i2cAddress,
                   &i2cStatus);
  while(i2cStatus == I2C1_MESSAGE_PENDING)
    {
      counter++;
      if (counter == 3000) // Usually it takes 300-400 counts to complete transaction
        {
          LED_SetHigh(); // I2C Error indication
          self->stat.flag.I2cTimeOut = true;
          break;
        }
    }
  if (i2cStatus == I2C1_MESSAGE_COMPLETE)
    {
      self->stat.flag.i2cErrorMessage = i2cStatus;
    }
  else
    {
      self->stat.flag.i2cErrorMessage = i2cStatus; // Error
    }
}

void I2C_Read(TMP175* self, uint8_t length)
{
  uint16_t counter = 0;
  I2C1_MESSAGE_STATUS i2cStatus = I2C1_MESSAGE_PENDING;
  self->stat.flag.i2cErrorMessage = i2cStatus;
  I2C1_MasterRead(self->rxBuffer,
                  length,
                  self->i2cAddress,
                  &i2cStatus);
  while(i2cStatus == I2C1_MESSAGE_PENDING)
    {
      counter++;
      if (counter == 3000) // Usually it takes 300-400 counts to complete transaction
        {
          LED_SetHigh(); // I2C Error indication
          self->stat.flag.I2cTimeOut = true;
          break;
        }
    }
  if (i2cStatus == I2C1_MESSAGE_COMPLETE)
    {
      self->stat.flag.i2cErrorMessage = i2cStatus;
    }
  else
    {
      self->stat.flag.i2cErrorMessage = i2cStatus; // Error
    }
}

// Mid level private functions

double ToCelsius(TMP175* self) // Convert sensor raw data to Celsius
{
  float tempC = 0.0;
  int16_t temp = self->temperatureReg[0];
  temp = temp << 8;
  temp = temp + self->temperatureReg[1];
  switch(self->confgurationReg.r1r0)
    {
    case CONF_R1R0_9BIT:
      temp = temp >> 7;
      tempC = (float) (temp / 2.0);
      break;
    case CONF_R1R0_10BIT:
      temp = temp >> 6;
      tempC = (float) (temp / 4.0);
      break;
    case CONF_R1R0_11BIT:
      temp = temp >> 5;
      tempC = (float) (temp / 8.0);
      break;
    case CONF_R1R0_12BIT:
      temp = temp >> 4;
      tempC = (float) (temp / 16.0);
      break;
    }
  self->celsius = tempC;
  sprintf(self->celsiusString, "%.2fC", self->celsius);
  return tempC;
}

// High level API Functions

void InitTMP175(TMP175* self)
{
  self->pointerReg = TEMP_REG;
  self->confgurationReg.tm = COMP_MODE_TM;
  self->confgurationReg.os = ONE_SHOT_NO;
  self->confgurationReg.r1r0 = CONF_R1R0_10BIT;
  self->confgurationReg.f1f0 = ONE_FAULT;
  self->confgurationReg.pol = POL_ALRTPIN_H;
  self->confgurationReg.sd = SHUTDOWN_NO;
  self->temperatureReg[0] = 0b00000000;
  self->temperatureReg[1] = 0b00000000;
  self->i2cAddress = TMP175I2C_ADDRESS;
  self->celsius = 0;
  //self->celsiusString={};
  //self->stat.byte = 0xFC;
  self->stat.flag.i2cErrorMessage = I2C1_MESSAGE_PENDING;
  self->stat.flag.I2cTimeOut = false;

  SetConfigReg(self, self->confgurationReg);
  DELAY_milliseconds(1); // Needed delay for I2C not to jam
  SetTHighReg(self, 0x1F, 0x00);
  DELAY_milliseconds(1);
  SetTLowReg(self, 0x1E, 0x00);
  DELAY_milliseconds(1);
  SetPointerReg(self, TEMP_REG);
}

void Tmp175ReadTemp(TMP175* self) // Prior this SetPointerReg to TempReg 
{
  I2C_Read(self, 2);
  self->temperatureReg[0] = self->rxBuffer[0];
  self->temperatureReg[1] = self->rxBuffer[1];
  ToCelsius(self);
}

void SetPointerReg(TMP175* self, pRegister pointerReg)
{
  self->pointerReg = pointerReg;
  self->txBuffer[0] = self->pointerReg;
  I2C_Write(self, self->txBuffer, 1);
}

void SetConfigReg(TMP175* self, confReg coRegvalue)
{
  self->confgurationReg = coRegvalue;
  self->txBuffer[0] = CONFIG_REG;
  self->txBuffer[1] = *((uint8_t *) & self->confgurationReg + 0);
  I2C_Write(self, self->txBuffer, 2);
}

void SetTHighReg(TMP175* self, uint8_t MSByte, uint8_t LSByte)
{
  self->tHighReg[0] = LSByte;
  self->tHighReg[1] = MSByte;
  self->txBuffer[0] = TEMP_HIGH_REG;
  self->txBuffer[1] = self->tHighReg[1];
  self->txBuffer[2] = self->tHighReg[0];
  I2C_Write(self, self->txBuffer, 3);
}

void SetTLowReg(TMP175* self, uint8_t MSByte, uint8_t LSByte)
{
  self->tLowReg[0] = LSByte;
  self->tLowReg[1] = MSByte;
  self->txBuffer[0] = TEMP_LOW_REG;
  self->txBuffer[1] = self->tLowReg[1];
  self->txBuffer[2] = self->tLowReg[0];
  I2C_Write(self, self->txBuffer, 3);
}

void ChangeResolution(TMP175* self, int8_t resolution)
{
  self->confgurationReg.r1r0 = resolution;
  SetConfigReg(self, self->confgurationReg);
}

void ChangeMode(TMP175* self, int8_t mode)
{
  self->confgurationReg.tm = mode;
  SetConfigReg(self, self->confgurationReg);
}


// Testing

void Write3Bytes(TMP175* self, int8_t pRegister, int8_t MSByte, int8_t LSByte)
{
  static uint8_t txBuffer[3] = {0, 0, 0}; // If not static is not working
  I2C1_MESSAGE_STATUS i2c1status = I2C1_MESSAGE_PENDING;
  txBuffer[0] = pRegister;
  txBuffer[1] = MSByte;
  txBuffer[2] = LSByte;
  I2C1_MasterWrite(txBuffer,
                   3,
                   self->i2cAddress,
                   &i2c1status);
  while(i2c1status != I2C1_MESSAGE_COMPLETE)
    {
    }
}












