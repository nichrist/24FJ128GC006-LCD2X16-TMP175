/* 
 * File:   app_usb_device.h
 * Author: Nick
 *
 * Created on January 10, 2018, 9:58 PM
 */

#ifndef APP_USB_DEVICE_H
#define	APP_USB_DEVICE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define eP1OutLength 64
#define eP1InLength 64

    typedef struct {
        USB_HANDLE EP1OUTEvenHandle;
        USB_HANDLE EP1OUTOddHandle;
        USB_VOLATILE uint8_t EP1OUTEvenBuffer[64];
        USB_VOLATILE uint8_t EP1OUTOddBuffer[64];
        uint8_t* pEP1OUTBuffer;
        bool EP1OUTEvenNeedsServicingNext; //true means even need servicing next, false means odd needs servicing next

        USB_HANDLE EP1INEvenHandle;
        USB_HANDLE EP1INOddHandle;
        USB_VOLATILE uint8_t EP1INEvenBuffer[64];
        USB_VOLATILE uint8_t EP1INOddBuffer[64];
        bool EP1INEvenNeedsServicingNext; //true means even need servicing next, false means odd needs servicing next
    } USB_APP_STRUCT;

    extern USB_APP_STRUCT usb;


    /* VARIABLES */
    extern USB_HANDLE EP1OUTEvenHandle;
    extern USB_HANDLE EP1OUTOddHandle;
    extern USB_VOLATILE uint8_t EP1OUTEvenBuffer[64];
    extern USB_VOLATILE uint8_t EP1OUTOddBuffer[64];
    extern uint8_t EP1OUTBuffer[64];
    extern bool EP1OUTEvenNeedsServicingNext; //true means even need servicing next, false means odd needs servicing next

    extern USB_HANDLE EP1INEvenHandle;
    extern USB_HANDLE EP1INOddHandle;
    extern USB_VOLATILE uint8_t EP1INEvenBuffer[64];
    extern USB_VOLATILE uint8_t EP1INOddBuffer[64];
    extern bool EP1INEvenNeedsServicingNext; //true means even need servicing next, false means odd needs servicing next

    /*********************************************************************
     * Function: void APP_DeviceVendorInit(void);
     * Overview: Setup Endpoints to be ready for tranfers
     * PreCondition: USB module is in CONFIGURED state
     * Input: None
     * Output: None
     ********************************************************************/
    void APP_DeviceVendorInit(void);

    void ArmEP1Out(void);
    void ArmEP1IN(void);


#ifdef	__cplusplus
}
#endif

#endif	/* APP_USB_DEVICE_H */

