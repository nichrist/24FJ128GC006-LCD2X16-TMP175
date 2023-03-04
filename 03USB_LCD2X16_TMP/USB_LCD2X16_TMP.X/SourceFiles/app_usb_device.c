#include "application.h"
#include "TMP175.h"

USB_APP_STRUCT usb = {// Not compulsory detailed initialization
  .EP1OUTEvenHandle = NULL,
  .EP1OUTOddHandle = NULL,
  .EP1INEvenHandle = NULL,
  .EP1INOddHandle = NULL,
};

extern uint16_t sl0ResultBuffer[];

void APP_DeviceVendorInit()
{
  // Now that USB is configured, enable the endpoints for use with the application and start the initial transfers
  USBEnableEndpoint(1, USB_OUT_ENABLED | USB_IN_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);

  //Prepare the OUT endpoints to receive the first packets from the host.
  usb.EP1OUTEvenHandle = USBTransferOnePacket(1, OUT_FROM_HOST, (uint8_t*) & usb.EP1OUTEvenBuffer, eP1OutLength); //First 64-bytes of data sent to EP1 OUT will arrive in the even buffer.
  usb.EP1OUTOddHandle = USBTransferOnePacket(1, OUT_FROM_HOST, (uint8_t*) & usb.EP1OUTOddBuffer, eP1OutLength); //Second 64-bytes of data sent to EP1 OUT will arrive in the odd buffer.
  usb.EP1OUTEvenNeedsServicingNext = true; //Used to keep track of which ping-pong buffer will contain the next sequential data packet.
  usb.pEP1OUTBuffer = (uint8_t*) usb.EP1OUTEvenBuffer; // Initialize pEP1OUTBuffer pointer to EP1OUTEvenBuffer
  usb.pEP1OUTBuffer[1] = 0x10; // In order APPLICATION STATE MACHINE show Temp Measurement on LCD

  //Prepare the IN endpoints to transmit the first packets to host.
  usb.EP1INEvenHandle = USBTransferOnePacket(1, IN_TO_HOST, (uint8_t*) & usb.EP1INEvenBuffer, eP1InLength);
  usb.EP1INOddHandle = USBTransferOnePacket(1, IN_TO_HOST, (uint8_t*) & usb.EP1INOddBuffer, eP1InLength);
  usb.EP1INEvenNeedsServicingNext = true; //Used to keep track of which buffer will contain the next sequential data packet.
}

/* Host sent data through EP1 OUT, process them!!!
 * Read the EP1 OUT appropriate (even or odd) buffer and then rearm it
 * First byte from PC contains desired Application State
 */
void ArmEP1Out(void)
{
  //Read the EP1 OUT appropriate (even or odd) buffers and then rearm it
  if (usb.EP1OUTEvenNeedsServicingNext == true)
    {
      if (!USBHandleBusy(usb.EP1OUTEvenHandle)) //Check if the endpoint has received any data from the host.
        {
          usb.pEP1OUTBuffer = (uint8_t*) usb.EP1OUTEvenBuffer;
          usb.EP1OUTEvenHandle = USBTransferOnePacket(1, OUT_FROM_HOST, (uint8_t*) & usb.EP1OUTEvenBuffer, eP1OutLength);
          usb.EP1OUTEvenNeedsServicingNext = false;
        }
    }
  else
    {
      if (!USBHandleBusy(usb.EP1OUTOddHandle)) //Check if the endpoint has received any data from the host.
        {
          //Re-arm the EP1OUTOdd BDT entry so the EP1OUTOddBuffer[] can receive
          //the second to next data packet sent by the host.
          usb.pEP1OUTBuffer = (uint8_t*) usb.EP1OUTOddBuffer;
          usb.EP1OUTOddHandle = USBTransferOnePacket(1, OUT_FROM_HOST, (uint8_t*) & usb.EP1OUTOddBuffer, eP1OutLength);
          usb.EP1OUTEvenNeedsServicingNext = true;
        }
    }
  APP.state = usb.pEP1OUTBuffer[0]; // First byte received from Host contains desired Application State
}

/* Called upon EP1_IN EVENT_TRANSFER. After HOST requested and received
 * data already stored in EP1INEven/OddBuffer fresh data can be loaded and read 
 * from HOST in it's following read request in EP1_IN pipe.
 * 
 * HOST Endpoint 1 transfer IN request (HOST want to read)
 * DEVICE response with the data stored in EP1OUTEvenBuffer and then load DATA0
 * HOST Endpoint 1 transfer IN request (HOST want to read)
 * DEVICE response with the data stored in EP1OUTOddBuffer and then load DATA1
 * HOST Endpoint 1 transfer IN request (HOST want to read) 
 * DEVICE response with the DATA0 stored in EP1OUTEvenBuffer and then load DATA2
 * HOST Endpoint 1 transfer IN request (HOST want to read) 
 * DEVICE response with the DATA1 stored in EP1OUTEvenBuffer and then load DATA3......
 */
void ArmEP1IN(void)
{
  if (usb.EP1INEvenNeedsServicingNext == true)
    {
      if (!USBHandleBusy(usb.EP1INEvenHandle)) // Make sure that the last transfer isn't busy by checking the handle
        {
          usb.EP1INEvenBuffer[0] = (unsigned int) APP.state;
          usb.EP1INEvenBuffer[1] = (unsigned int) myTMP175.temperatureReg[0];
          usb.EP1INEvenBuffer[2] = (unsigned int) myTMP175.temperatureReg[1];
          //            EP1INEvenBuffer[3] = (unsigned int) sl0ResultBuffer[0];
          //            EP1INEvenBuffer[4] = (unsigned int) (sl0ResultBuffer[0] >> 8);
          //            EP1INEvenBuffer[5] = (unsigned int) sl0ResultBuffer[1];
          //            EP1INEvenBuffer[6] = (unsigned int) (sl0ResultBuffer[1] >> 8);
          //            EP1INEvenBuffer[7] = 0x07;

          //EP1INEvenBuffer[7] = (unsigned int) (ADC_AN5 >> 8); //left_byte
          usb.EP1INEvenHandle = USBTransferOnePacket(1, IN_TO_HOST, (uint8_t*) & usb.EP1INEvenBuffer, eP1InLength); //Prepares (Arm) EP1In 
          //so that it may send the above data to the host in a future IN transaction initiated by the host.
          usb.EP1INEvenNeedsServicingNext = false;
        }
    }
  else
    {
      if (!USBHandleBusy(usb.EP1INOddHandle))
        {
          usb.EP1INOddBuffer[0] = (unsigned int) APP.state;
          usb.EP1INOddBuffer[1] = (unsigned int) myTMP175.temperatureReg[0];
          usb.EP1INOddBuffer[2] = (unsigned int) myTMP175.temperatureReg[1];
          //            EP1INOddBuffer[3] = (unsigned int) sl0ResultBuffer[0];
          //            EP1INOddBuffer[4] = (unsigned int) (sl0ResultBuffer[0] >> 8);
          //            EP1INOddBuffer[5] = (unsigned int) sl0ResultBuffer[1];
          //            EP1INOddBuffer[6] = (unsigned int) (sl0ResultBuffer[1] >> 8);
          //            EP1INOddBuffer[7] = 0x00;

          usb.EP1INOddHandle = USBTransferOnePacket(1, IN_TO_HOST, (uint8_t*) & usb.EP1INOddBuffer, eP1InLength);
          usb.EP1INEvenNeedsServicingNext = true;
        }
    }
}