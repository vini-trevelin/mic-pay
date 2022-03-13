/*
 * delayT1.c
 *
 * Created: 12/03/2022 13:29:31
 *  Author: VITOR
 */ 

//////////////////////////////////////////////////////////////////////////
							//INCLUDES//
//////////////////////////////////////////////////////////////////////////
#include<avr/interrupt.h>
#include <avr/io.h>
#include "delayT1.h"

//////////////////////////////////////////////////////////////////////////
							//FUNÇÕES//
//////////////////////////////////////////////////////////////////////////

void enableDelayT1(){
	TCNT1  = 0;								// Inicia em zero
	OCR1A  = 16000;							// 16000 mil contagens = 1ms = tempo do  delay
	TIMSK0 = (1<<TOIE0);					// Habilita interrupção por overflow timer0
	sei();									// Habilita interrupções de maneira geral
}

void delay(){								//TIMER1 PARA 1ms -> a 16 Mhz -> 16000 contagens
	//Vou utilizar o comparador
	TCCR1B = 0x01;							//Liga o timer
	while((TIFR1&(1<<0)) == 0);				//TESTA ESTOURO
	TCCR1B = 0x00;							//Desliga o Timer
	TIFR1 |= (1<<0);						//LIMPA FLAG DE ESTOURO
	
}