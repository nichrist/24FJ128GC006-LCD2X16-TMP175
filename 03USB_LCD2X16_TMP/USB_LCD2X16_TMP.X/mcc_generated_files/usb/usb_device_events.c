// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright 2015 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
 *******************************************************************************/
//DOM-IGNORE-END

/** INCLUDES *******************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "usb_device.h"
#include "usb_device_generic.h"
#include "../../SourceFiles/app_usb_device.h"
#include "../../SourceFiles/application.h"

/*******************************************************************
 * Function:        bool USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, uint16_t size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  uint16_t size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
bool USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, uint16_t size)
{
    USB_VOLATILE USTAT_FIELDS *USTATlocal;
    uint8_t endpoint_numberlocal;
    uint8_t endpoint_direction;

    switch((int) event)
      {
        case EVENT_TRANSFER:
          /* A USB transfer (In or Out) of an endpoint other that 0 occurred */
          USTATlocal = pdata;
          endpoint_numberlocal = USTATlocal->endpoint_number;
          endpoint_direction = USTATlocal->direction;
          if(USTATlocal->endpoint_number == 1 && USTATlocal->direction == 0) // Endpoint 1 transfer OUT
          {
              ArmEP1Out();  // Host sent data through EP1 OUT, process them!!!
          }
          else if(USTATlocal->endpoint_number == 1 && USTATlocal->direction == 1) // Endpoint 1 transfer IN
          {
              ArmEP1IN();   // Host requested data from EP1 IN load new data and rearm Endpoint!!!
          }
          else
              Nop();
          /* A USB transfer has completed.
           */
          break;

        case EVENT_SUSPEND:
          //Call the hardware platform specific handler for suspend events for
          //possible further action (like optionally going reconfiguring the application
          //for lower power states and going to sleep during the suspend event).  This
          //would normally be done in USB compliant bus powered applications, although
          //no further processing is needed for purely self powered applications that
          //don't consume power from the host.
          break;

        case EVENT_RESUME:
          //Call the hardware platform specific resume from suspend handler (ex: to
          //restore I/O pins to higher power states if they were changed during the 
          //preceding SYSTEM_Initialize(SYSTEM_STATE_USB_SUSPEND) call at the start
          //of the suspend condition.
          break;

        case EVENT_CONFIGURED:
          APP_DeviceVendorInit();
          break;

        case EVENT_SET_DESCRIPTOR:
          break;

        case EVENT_EP0_REQUEST:
          /* We have received a non-standard USB request.  The vendor driver
           * needs to check to see if the request was for it. */
          USBCheckVendorRequest();
          break;

        case EVENT_BUS_ERROR:
          break;

        case EVENT_TRANSFER_TERMINATED:
          break;

        default:
          break;
      }
    return true;
}

void __attribute__((interrupt,auto_psv)) _USB1Interrupt()
{
    USBDeviceTasks();
}
