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
#include <p18f4550.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <stddef.h>
#include <ctype.h>
#include <assert.h>
#include "xlcd.h"
#include "usart.h"
#include "Interrupt.h"
#include "timers.h"

//#include <i2c.h>
//#include <math.h>
					// I2C library functions
/******************************************** CONFIGURATION **************************************************/

#pragma config PLLDIV   = 5          // (20 MHz crystal on board)
#pragma config CPUDIV   = OSC1_PLL2  
//#pragma config USBDIV   = 2          // Clock source from 96MHz PLL/2
#pragma config FOSC     = HSPLL_HS
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config PWRT     = OFF
// #pragma config PWRT     = ON
#pragma config BOR      = ON
#pragma config BORV     = 2
#pragma config VREGEN   = OFF       //USB Voltage Regulator
#pragma config WDT      = OFF
// #pragma config WDT      = ON
#pragma config WDTPS    = 32768
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
#pragma config PBADEN   = ON
//#pragma config CCP2MX   = ON
#pragma config STVREN   = ON	    // reset if stack overflow	
#pragma config LVP      = OFF
// #pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming
#pragma config XINST    = OFF       // Extended Instruction Set
 #pragma config CP0      = OFF     // code protect 
//#pragma config CP0      = ON
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF
#pragma config CPB      = OFF
//#pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
//#pragma config WRT2     = OFF
//#pragma config WRT3     = OFF
#pragma config WRTB     = OFF       // Boot Block Write Protection
#pragma config WRTC     = OFF
//#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
//#pragma config EBTR2    = OFF
//#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF

//DEFINES
#define LED_GREEN   LATBbits.LATB5
#define LED_RED     LATBbits.LATB4
#define LED_GREEN2  LATBbits.LATB3
#define RELAY       LATBbits.LATB2
#define ON          1
#define OFF         0

// Init MCU
void InitMCU()
{
    //Ios
    TRISBbits.TRISB5 = 0;
    PORTBbits.RB5 = 0;
    TRISBbits.TRISB4 = 0;
    PORTBbits.RB4 = 0;
    TRISBbits.TRISB3 = 0;
    PORTBbits.RB3 = 0;
    TRISBbits.TRISB2 = 0;
    PORTBbits.RB2 = 0;
    
    //Serial port
    initUsart();
    //Init interrupts timer
    InitTimer();
}

void  ledStatus()
{
    static StructTimer  timerLedStatus;
    //Check timer status
    if(timerLedStatus.flgInitTimer == FALSE){
        //Init timer 3 seconds 
        timerInit(&timerLedStatus, 3, &sSystemTimers.timer1s);
        LED_GREEN =~ LED_GREEN;  
        putsUsart((char *)(LED_GREEN == ON ? "\n\rLED GREEN ON" : "\n\rLED GRENN OFF"));
    }
    else if(CheckTimeout(timerLedStatus) == TRUE){      
        
        timerLedStatus.flgInitTimer = FALSE;
    }
}

void  ledStatus1()
{
    static StructTimer  timerLedStatus;

    //Check timer status
    if(timerLedStatus.flgInitTimer == FALSE){
        //Init timer 2 seconds 
        timerInit(&timerLedStatus, 20, &sSystemTimers.timer1ms);
        LED_RED =~ LED_RED;  
        putsUsart((char *)(LED_RED == ON ? "\n\rLED RED ON" : "\n\rLED RED OFF"));    
    }
    else if(CheckTimeout(timerLedStatus) == TRUE){      
        
        timerLedStatus.flgInitTimer = FALSE;
    }
}

void  ledStatus2()
{
    static StructTimer  timerLedStatus;
    //Check timer status
    if(timerLedStatus.flgInitTimer == FALSE){
        //Init timer 1 seconds 
        timerInit(&timerLedStatus, 5, &sSystemTimers.timer100ms);
        LED_GREEN2 =~ LED_GREEN2;
        putsUsart((char *)(LED_GREEN2 == ON ? "\n\rLED GREEN2 ON" : "\n\rLED GREEN2 OFF"));    
    }
    else if(CheckTimeout(timerLedStatus) == TRUE){      
        timerLedStatus.flgInitTimer = FALSE;
    }
}

void  relay()
{
    static StructTimer  timerRelay;
    //Check timer status
    if(timerRelay.flgInitTimer == FALSE){
        //Init timer 1 minute 
        timerInit(&timerRelay, 1, &sSystemTimers.timer1hour);
        RELAY =~ RELAY;
        putsUsart((char *)(RELAY == ON ? "\n\rRELAY ON" : "\n\rRELAY OFF"));    
    }
    else if(CheckTimeout(timerRelay) == TRUE){      
        
        timerRelay.flgInitTimer = FALSE;
    }
}

void  KeepAlive()
{
    static StructTimer  timerKeepAlive;
    //Check timer status
    if(timerKeepAlive.flgInitTimer == FALSE){
        
        timerInit(&timerKeepAlive, 10, &sSystemTimers.timer1s);
        putsUsart((char *)"\n\rKeep Alive ...");
    }
    else if(CheckTimeout(timerKeepAlive) == TRUE){      
        timerKeepAlive.flgInitTimer = FALSE;
    }
}

void main(void)
{
    InitMCU();
    putsUsart((char *)"\n\rSucess init device!!");
    
    while(1)
    {
        //Task blink status led
        ledStatus();
        ledStatus1();
        ledStatus2();
        relay();
        //Task Send serial keep alive baud 115200
        KeepAlive();
    }
}