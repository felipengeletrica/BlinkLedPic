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

 FileName:      usart.c
 Dependencies:  See INCLUDES section
 Processor:     PIC18F4550
 Hardware:	NanoAmperimetro
 Compiler:  	Microchip XC18 Version 1.2 or later
 programmer:    Felipe Vargas

/************* INCLUDES ********************************************/
#include <p18f4550.h>

#include "usart.h"

//init the serial port UART 115200@48MHz, 8, N, 1)
void initUsart(void)
{
        
/*	// Setup the USART for 19200 baud @ 48MHz
	SPBRGH = 0;
	SPBRG  = 38;                      // 19200 baud @ 48MHz
	RCSTA  = 0b10010000;             // setup USART receive  SPEN|RX9|SREN|CREN|ADDEN|FERR|OERR|RX9D
	TXSTA  = 0b00100000;             // setup USART transmit CSRC|TX9|TXEN|SYNC|SENDB|BRGH|TRMT|TX9D
	
*/	
	// Set the port direction.
	TRISCbits.TRISC6 = 0;
	TRISCbits.TRISC7 = 1;
	
	// Setup the USART for 115200 baud @ 48MHz
	BAUDCON = 0b00001000; //ABDOVF|RCIDL|RXDTP|TXCKP|BRG16|—|WUE|ABDEN
	SPBRGH  = 0;
	SPBRG   = 25;	    
	RCSTA   = 0b10010000; //setup USART receive  SPEN|RX9|SREN|CREN|ADDEN|FERR|OERR|RX9D
	TXSTA   = 0b00100000; //setup USART transmit CSRC|TX9|TXEN|SYNC|SENDB|BRGH|TRMT|TX9D
}


// Send USART CHAR
void putUsart(char c)
{
    while (!TXSTAbits.TRMT); // TSR register is empty
    TXREG = c; 	
}

// Get
char getUsart(void)
{
    while (!PIR1bits.RCIF);
    return RCREG; // read new data into variable
}

//
void putsUsart(char *s)
{

    while (*s)      // loop until *s == '\0', end of string
        putUsart(*s++); //send caracter and point to next one
}


char *getsUsart(char *s, int len)
{

    char *p = s; //copy buffer to pointer
    do
    {
        *s = getUsart(); //wait for new carctere
        if(*s == '\r')
            break;
        s++;
        len--;
    }while(len > 1); //until buffer full

    *s = '\0';

    return p;  //return buffer pointer
}
