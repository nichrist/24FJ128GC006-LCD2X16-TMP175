/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24FJ128GC006
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
 */

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

/**
  Section: Included Files
 */
#include "../SourceFiles/application.h"

#include <TMP175.h>
#include <LCD16C.h>

static char message[16];

void TMR1_CallBack(void)
{
    LED_Toggle();
    LcdSetCursorXY(1, 2);
    sprintf(message, "%s\n", getStateName(APP.state));
    LcdWriteString(message);
}

// <editor-fold defaultstate="expanded" desc="MAIN ROUTINE">

int main(void)
{
    SYSTEM_Initialize();
    InitLCD2X16(&myLCD2X16);
    InitTMP175(&myTMP175);
    TMR1_Start();
    APP.state = TEST_TMP175;
    LcdWriteString("Init OK");
    DELAY_milliseconds(1000);
    while (1)
    {
        // <editor-fold defaultstate="collapsed" desc="APPLICATION STATE MACHINE">
        switch (APP.state)
        {
        case DO_NOTHING:
            Nop();
            break;
        case STATE_1:
            LcdClear();
            APP.state = DO_NOTHING;
            break;
        case STATE_2:
            LcdHome();
            APP.state = DO_NOTHING;
            break;
        case STATE_3:
            LcdWriteChar(usb.pEP1OUTBuffer[1]);
            APP.state = DO_NOTHING;
            break;
        case TEST_TMP175:
            if (usb.pEP1OUTBuffer[1] == 0x00)
            {
                SetTHighReg(&myTMP175, usb.pEP1OUTBuffer[2], usb.pEP1OUTBuffer[3]);
            }
            else if (usb.pEP1OUTBuffer[1] == 0x01)
            {
                SetPointerReg(&myTMP175, usb.pEP1OUTBuffer[2]);
            }
            else if (usb.pEP1OUTBuffer[1] == 0x02)
            {
                ChangeResolution(&myTMP175, usb.pEP1OUTBuffer[2]);
            }
            else if (usb.pEP1OUTBuffer[1] == 0x03)
            {
                ChangeMode(&myTMP175, usb.pEP1OUTBuffer[2]);
            }
            else
            {
                Tmp175ReadTemp(&myTMP175);
                DELAY_milliseconds(5);
                LcdSetCursorXY(1, 1);
                sprintf(message, "%.2f", myTMP175.celsius);
                LcdWriteString(message);
                LcdWriteChar(0xDF); // Celsius circle symbol
                LcdWriteChar('C');
            }
            DELAY_milliseconds(500);
            break;
        case STATE_5:
            break;
        case STATE_6:
            APP.state = DO_NOTHING;
            break;
        case STATE_7:
            DELAY_milliseconds(500);
            break;
        default:
            // ERROR
            break;
        }
        // </editor-fold>
    }
    return 1;
}
// </editor-fold>

/**
 End of File
 */

