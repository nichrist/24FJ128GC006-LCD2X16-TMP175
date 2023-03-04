/* 
 * File:   system.h
 * Author: Nick
 *
 * Created on January 12, 2018, 11:00 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>  // sprintf declaration

#include "../mcc_generated_files/clock.h"
#include "../mcc_generated_files/pin_manager.h"
#include "../mcc_generated_files/system.h"
#include "../mcc_generated_files/delay.h"
#include "../mcc_generated_files/tmr1.h"
#include "../mcc_generated_files/interrupt_manager.h"
#include "../mcc_generated_files/traps.h"
#include "../mcc_generated_files/usb/usb.h"
#include "../SourceFiles/app_usb_device.h"


typedef enum {
    DO_NOTHING = 0,
    STATE_1 = 1,
    STATE_2 = 2,
    STATE_3 = 3,
    TEST_TMP175 = 4,
    STATE_5 = 5,
    STATE_6 = 6,
    STATE_7 = 7
} APPLICATION_STATE;

typedef struct {
    APPLICATION_STATE state;
    APPLICATION_STATE prvState;
} APP_STRUCT;

extern APP_STRUCT APP;

char* getStateName(APPLICATION_STATE state);

#endif	/* SYSTEM_H */

