/********************************************************************
Copyright (c) 2014-2017, Felipe Vargas <felipeng.eletrica@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.

/********************************************* INCLUDES *******************************************************/
#include "timers.h"


//Initializes the timer with the timer value and global resolution
int timerInit(StructTimer* ptrTimer, const uint16_t timeoutValue, const uint16_t *ptrSystemTimerValue)
{
    int8_t ret = FALSE;
    
    if(timeoutValue < MAX_VALUE){

        ptrTimer->timeoutValue = timeoutValue;
        //Indicates that the timer has started
        ptrTimer->flgInitTimer =  TRUE;
        ptrTimer->ptrSystemTimerValue = (uint16_t *) ptrSystemTimerValue;
        //Start the timer copy timestamp
        ptrTimer->startTime = *(ptrTimer->ptrSystemTimerValue);
        ret = TRUE;
    }
    
    return ret;
}

//Checks if the timer has already passed with timer overflow correction 
//by calculating the difference
static uint16_t timerValue(const StructTimer timerStruct)
{
    typedef_timers difTime = 0;

    if(!timerStruct.timeoutValue)
    {
        return(0);
    }

	difTime = (*(timerStruct.ptrSystemTimerValue)  < timerStruct.startTime)
	? (MAX_VALUE - timerStruct.startTime) + *(timerStruct.ptrSystemTimerValue)
	: *(timerStruct.ptrSystemTimerValue) - timerStruct.startTime; 
    
	return(difTime);
}

//Check for timeout
int CheckTimeout(StructTimer timerStruct)
{
    //It reached the programd time
    return(timerValue(timerStruct)  >= timerStruct.timeoutValue) ? TRUE : FALSE;
}

void SystemTimers()
{  
    //Checks and increases the values of the timer from 1 ms
    sSystemTimers.timer1ms == MAX_VALUE 
            ? (sSystemTimers.timer1ms = 0) 
            : sSystemTimers.timer1ms++;
    
    //Increase the timer of 10 ms with protection from overflow
    if(!(sSystemTimers.timer1ms % 10)){
        sSystemTimers.timer10ms == MAX_VALUE 
                ? (sSystemTimers.timer10ms = 0) 
                : sSystemTimers.timer10ms++;
    }
    //Increase the timer of 100 ms with protection from overflow
    if(!(sSystemTimers.timer1ms % 100)){
                sSystemTimers.timer100ms == MAX_VALUE 
                        ? (sSystemTimers.timer100ms = 0) 
                        : sSystemTimers.timer100ms++;
    }
    //Increase the timer of 1s with protection from overflow
    if(!(sSystemTimers.timer1ms % 1000)){
                sSystemTimers.timer1s == MAX_VALUE 
                        ? sSystemTimers.timer1s = 0 
                        : sSystemTimers.timer1s++;
    }
    //Increase the timer of 1 min with protection from overflow
    if(!(sSystemTimers.timer1ms % 1000) && !(sSystemTimers.timer1s % 60)){
                sSystemTimers.timer1min == MAX_VALUE 
                        ? sSystemTimers.timer1min = 0 
                        : sSystemTimers.timer1min++;
    }
    //Increase the timer of one hour with protection from overflow
    if(!(sSystemTimers.timer1ms % 60000) && !(sSystemTimers.timer1min % 60) ){
                sSystemTimers.timer1hour == MAX_VALUE 
                        ? sSystemTimers.timer1hour = 0 
                        : sSystemTimers.timer1hour++;
    }
}
