#ifndef _COMUM_H
#define	_COMUM_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <assert.h>

/******************************************** DEFINES **************************************************/
//Controls
#define TRUE   1 //Data type
#define FALSE  0 //Data type

//Defines length circular buffers depends processados words 
#define MAX_VALUE       UINT16_MAX  //Maximum value
#define typedef_timers  uint16_t    //Depends Maximum value

/********************************************* STRUCTS *******************************************************/
//Struct circular timers resolutions 
typedef struct {
    typedef_timers timer1ms;         ///< Timer 1 ms
    typedef_timers timer10ms;        ///< Timer 10 ms
    typedef_timers timer100ms;       ///< Timer 100 ms
    typedef_timers timer1s;          ///< Timer 1 s
    typedef_timers timer1min;        ///< Timer 1 min
    typedef_timers timer1hour;       ///<timer  1 hour
} StructCircTimers;

//Struct to control timers 
typedef struct {
    typedef_timers    startTime;            ///< Timestamp initial value
    typedef_timers    timeoutValue;         ///< Setup time
    uint8_t           flgInitTimer;         ///< flag init timer is init
    typedef_timers*   ptrSystemTimerValue;  ///< System timers
} StructTimer;

/********************************************* VARIABLES  *******************************************************/
StructCircTimers sSystemTimers = {0, 0, 0, 0 ,0}; //Initializer timers

/********************************************* PROTOTYPES *******************************************************/

int timerInit(StructTimer* ptrTimer, const uint16_t timeoutValue, const uint16_t *ptrSystemTimerValue);
int CheckTimeout(const StructTimer timerStruct);

void SystemTimers(void);
#endif	/* _COMUM_H */