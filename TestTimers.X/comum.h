#ifndef _COMUM_H
#define	_COMUM_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <assert.h>

#define TRUE   1 //Tipo de dado
#define FALSE  0 //Tipo de dado

#define MAX_VALUE  UINT16_MAX  //Maximo valor de contagem dos timers


//Estrutura dos temporizadores do sistema
typedef struct {
    uint16_t timer1ms;         ///< Timer 1 ms
    uint16_t timer10ms;        ///< Timer 10 ms
    uint16_t timer100ms;       ///< Timer 100 ms
    uint16_t timer1s;          ///< Timer 1 s
    uint16_t timer1min;        ///< Timer 1 min
    uint16_t timer1hour;       ///<timer  1 hour
} sSystemTimers_t;

///Estrutura de um temporizador do firmware
typedef struct {
    uint16_t     startTime;    ///< timestamo de valor inicial
    uint16_t     timeoutValue; ///< Valor para ocorrer o timeout do temporizador
    uint8_t      flgInitTimer; ///< flag de timer inicializado
    uint16_t*    ptrSystemTimerValue; ///< Ponteiro para o timer do sistema
} sTimer_t;

//Retorno das funções do timer
typedef enum {
    timerTimeout,   ///< Temporizador atingiu o tempo programado
    timerNotTimeout ///< Temporizador não atingiu o tempo programado
} timerFuncReturn;


sSystemTimers_t sSystemTimers = {0, 0, 0, 0 ,0}; //Temporizadores do firmware com resolução de 1 ms

int timerInit(sTimer_t* ptrTimer, const uint16_t timeoutValue, const uint16_t *ptrSystemTimerValue);
timerFuncReturn timerIsTimeout(const sTimer_t timerStruct);
void SystemTimers(void);
#endif	/* _COMUM_H */