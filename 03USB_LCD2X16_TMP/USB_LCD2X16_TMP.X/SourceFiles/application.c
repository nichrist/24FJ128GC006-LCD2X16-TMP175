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
#include "application.h"

/** VARIABLES **/
APP_STRUCT APP = {// Not compulsory detailed initialization
    .state = DO_NOTHING,
    .prvState = DO_NOTHING,
};

char* getStateName(APPLICATION_STATE state)
{
    switch (state)
    {
    case DO_NOTHING: return "DO_NOTHING";
    case STATE_1: return "STATE_1";
    case STATE_2: return "STATE_2";
    case STATE_3: return "STATE_3";
    case TEST_TMP175: return "TEST_TMP175";
    case STATE_5: return "STATE_5";
    case STATE_6: return "STATE_6";
    case STATE_7: return "STATE_7";
//    case STATE_8: return "STATE_8";
//    case STATE_9: return "STATE_9";
//    case STATE_10: return "STATE_10";
//    case STATE_11: return "STATE_11";
//    case STATE_12: return "STATE_12";
//    case STATE_13: return "STATE_13";
    default: return "UNKNOWN STATE";
    }
}






