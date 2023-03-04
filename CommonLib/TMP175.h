/* 
 * File:   TMP175.h
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

#ifndef TMP175_H
#define	TMP175_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CONF_R1R0_9BIT       0
#define CONF_R1R0_10BIT      1
#define CONF_R1R0_11BIT      2
#define CONF_R1R0_12BIT      3
#define ONE_FAULT      0b00
#define TWO_FAULT      0b01
#define THREE_FAULT    0b10
#define FOUR_FAULT     0b11
#define ONE_SHOT_YES   1
#define ONE_SHOT_NO    0
#define COMP_MODE_TM   0
#define INTR_MODE_TM   1
#define POL_ALRTPIN_L  0
#define POL_ALRTPIN_H  1
#define SHUTDOWN_YES   1
#define SHUTDOWN_NO    0

#define TMP175I2C_ADDRESS       0x002F  // // 00101111

    //Pointer Register Addresses

    typedef enum {
        TEMP_REG = 0,
        CONFIG_REG = 1,
        TEMP_LOW_REG = 2,
        TEMP_HIGH_REG = 3,
    } pRegister;

    typedef struct confReg {
        unsigned int sd : 1;
        unsigned int tm : 1;
        unsigned int pol : 1;
        unsigned int f1f0 : 2;
        unsigned int r1r0 : 2;
        unsigned int os : 1;
    } confReg;

    typedef union {

        struct {
            uint8_t i2cErrorMessage : 2;
            uint8_t I2cTimeOut : 1;
            uint8_t I2c1 : 1;
            uint8_t I2c2 : 1;
            uint8_t I2c3 : 1;
            uint8_t I2c4 : 1;
            uint8_t bit6 : 1;
        } flag;
        uint8_t byte;
    } TMP175_STATUS;

    typedef struct TMP175 {
        pRegister pointerReg;
        uint8_t temperatureReg[2];
        confReg confgurationReg;
        uint8_t tLowReg[2];
        uint8_t tHighReg[2];
        uint8_t i2cAddress;        
        uint8_t txBuffer[4];
        uint8_t rxBuffer[4];
        TMP175_STATUS stat;
        double celsius;
        char celsiusString[10]; // Be careful when sprintf write here not to exceed the 10 Bytes storage. Else will corrupt the following member variables
    } TMP175;

    /*
     * Object instance: myTMP175
     * ----------------------------
     *   TMP175 Instance object to be used in initialization
     *   e.g. InitTMP175(&myTMP175)
     */
    extern TMP175 myTMP175;
    /*
     * Function: InitTMP175
     * ----------------------------
     *   Initialize a TMP175 Instance object
     *
     *   self: TMP175 Instance object address
     *
     *   returns: void
     */
    void InitTMP175(TMP175* self);
    void Tmp175ReadTemp(TMP175* self);

    void ChangeResolution(TMP175* self, int8_t resolution);
    void ChangeMode(TMP175* self, int8_t mode);

    void SetPointerReg(TMP175* self, pRegister pointerReg);
    void SetConfigReg(TMP175* self, confReg coRegvalue);
    void SetTHighReg(TMP175* self, uint8_t MSByte, uint8_t LSByte);
    void SetTLowReg(TMP175* self, uint8_t MSByte, uint8_t LSByte);


    void Write3Bytes(TMP175* self, int8_t pRegister, int8_t MSByte, int8_t LSByte);


#ifdef	__cplusplus
}
#endif

#endif	/* TMP175_H */

