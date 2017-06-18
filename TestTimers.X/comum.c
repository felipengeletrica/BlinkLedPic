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

#include "comum.h"


void timerInit(sTimer_t* ptrTimer, const uint16_t timeoutValue, const uint16_t *ptrSystemTimerValue)
{
    ptrTimer->timeoutValue = timeoutValue;
    //Indica que o timer foi inicializado
    ptrTimer->flgInitTimer = (timeoutValue >= MAX_VALUE) ? FALSE : TRUE;
    ptrTimer->ptrSystemTimerValue = (uint16_t *) ptrSystemTimerValue;
    //Inicia a temporização
    ptrTimer->startTime = *(ptrTimer->ptrSystemTimerValue);
}


static uint16_t timerValue(const sTimer_t timerStruct)
{
    uint16_t difTime;

    if(!timerStruct.timeoutValue)
    {
        return(0);
    }

	difTime = (*(timerStruct.ptrSystemTimerValue)  < timerStruct.startTime)
	? (MAX_VALUE - timerStruct.startTime) + *(timerStruct.ptrSystemTimerValue)
	: *(timerStruct.ptrSystemTimerValue) - timerStruct.startTime; 
    
	return(difTime);
}

timerFuncReturn timerIsTimeout(sTimer_t timerStruct)
{
    //Verifica atingiu o tempo programado
    if(timerValue(timerStruct)  >= timerStruct.timeoutValue)
    {

        return(timerTimeout);
    }
    
    return(timerNotTimeout);
}

void SystemTimers()
{  
    //Verifica e incrementa os valores do timer de referência de 1 ms
    sSystemTimers.timer1ms == MAX_VALUE 
            ? (sSystemTimers.timer1ms = 0) 
            : sSystemTimers.timer1ms++;
     
    //Incrementar o timer de 10 ms com protecao de overflow
    if(!(sSystemTimers.timer1ms % 10))
        sSystemTimers.timer10ms == MAX_VALUE 
                ? (sSystemTimers.timer10ms = 0) 
                : sSystemTimers.timer10ms++;
    
    //Incrementar o timer de 100 ms com protecao de overflow
    if(!(sSystemTimers.timer1ms % 100))
                sSystemTimers.timer100ms == MAX_VALUE 
                        ? (sSystemTimers.timer100ms = 0) 
                        : sSystemTimers.timer100ms++;
    
    //Incrementar o timer de 1 s com protecao de overflow
    if(!(sSystemTimers.timer1ms % 1000))
                sSystemTimers.timer1s == MAX_VALUE 
                        ? sSystemTimers.timer1s = 0 
                        : sSystemTimers.timer1s++;
    
    //Incrementar o timer de 1 minuto com protecao de overflow
    if(!(sSystemTimers.timer1s % 60))
                sSystemTimers.timer1min == MAX_VALUE 
                        ? sSystemTimers.timer1min = 0 
                        : sSystemTimers.timer1min++;
}
