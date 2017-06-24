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

 * /********************************************************************
 FileName:      Interrupt.c
 Dependencies:  See INCLUDES section
 Processor:     PIC18F4550
 Compiler:  	Microchip XC18 Version 1.2 or later
/********************************************* INCLUDES *******************************************************/

#include <p18f4550.h>
#include "Interrupt.h"
#include "timers.h"


void InitTimer()
{
    T1CON = 0x01;           //Configure Timer1 interrupt
    PIE1bits.TMR1IE = 0;
    INTCONbits.PEIE = 1;
    RCONbits.IPEN=0x01;
    IPR1bits.TMR1IP=0x01;   // TMR1 high priority ,TMR1 Overflow Interrupt Priority bit
    INTCONbits.GIE = 1;
    PIR1bits.TMR1IF = 0;
    T0CON=0X00;
    INTCONbits.T0IE = 1;    // Enable interrupt on TMR0 overflow
    INTCON2bits.TMR0IP=0x00;
    T0CONbits.TMR0ON = 1;
}

//High priority interrupt vector
void interrupt tc_int(void) 
{
    //interrupcao timer 1
    if (TMR1IE && TMR1IF)
    {
        TMR1IF=0;
    }
}

//Low priority interrupt vector
void interrupt low_priority   LowIsr(void)
{
    //Check interrupt is timer 0 bit
    if(INTCONbits.T0IF && INTCONbits.T0IE)
    {
        //INTCONbits.GIE = 0;   //Disable global interrupt
        TMR0 = 59535;           //Reload the timer - 1ms per interrupt
        INTCONbits.T0IF = 0;    //Clear the interrupt flag
        //Process struct timers
        SystemTimers();
        //INTCONbits.GIE = 1;   //Enable global interrup
    }
    
    //Check interrupt is timer 1 bit
    //if (TMR1IE && TMR1IF)
    //{
    //   TMR1IF=0;
    //}
    
    //ios ...
}

























/**
void interrupt tc_int(void)             // High priority interrupt
{
    if (TMR1IE && TMR1IF)
    {
        TMR1IF=0;      
    }
}

void interrupt low_priority   LowIsr(void)    //Low priority interrupt
{
    //interupção do timer 0
    if(INTCONbits.T0IF && INTCONbits.T0IE)  // If Timer flag is set & Interrupt is enabled
    {
        TMR0 -= 250;                    // Reload the timer - 250uS per interrupt
        INTCONbits.T0IF = 0;            // Clear the interrupt flag
      
    }
    //Interrupcao do timer 1
    if (TMR1IE && TMR1IF)
    {
        TMR1IF=0;
    }
}
 */