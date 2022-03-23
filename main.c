
//////////////////////////////////////////////////////////////////////////
						//INCLUDES//
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Modulos/botoes.c"
#include "Modulos/delayT1.c"
#include "Modulos/serial.c"
#include "Modulos/lcd.c"

//#define TESTE 1 //comentar para não fazer o main


//////////////////////////////////////////////////////////////////////////
						//INTERUPÇÕES//
//////////////////////////////////////////////////////////////////////////


#ifdef TESTE 
	ISR(TIMER0_OVF_vect){						// Interrupcao por overflow do TIMER0
		if(freq!=TECLA_INVALIDA){
			TCNT0 =freq*20;
			PORTB ^= (1 << 5);
			TIFR0 |= (1<<0);						//limpa estouro
		}
	}
#endif

int main(){
	// Se começar a escrever 1 sem parar no inicio é só dar Reload que volta funcionar
	
	//USART_INIT(UBRR);
	lcd_init_4d();
	
	char teclaLida =  TECLA_INVALIDA;			// inicia como uma tecla invalida
	//setupTeclado();							// inicializa o teclado
	//enableDelayT1();							// para usar o timer1 para delay-> Temos que ver se não vamos usar aquela função pronta dai liberariamos esse timer
	//enableTestTeclado();
	
	//char i=65; 
	//char s2[] = "o cliente deve:";
	//char sfinal[10];
	
	while (1){
		
		teclaLida = valor_tecla(tecla_lida());
		_delay_ms(2000); // talvez ocorram alguns bugs pelo valor disso aq estar alto, coloquei pq n fiz o debounce
		
		/*
		sprintf(sfinal,"%d",i); //codigozinho pra escrever as variaves no lcd
		writeString(s2);
		writeInstruction(lcd_LineTwo | lcd_SetCursor);
		writeString(sfinal);
		*/
		
		//teclaLida = varrerTeclado();
		//if (teclaLida!=TECLA_INVALIDA){
		//	execCmdTecla(teclaLida);
		//	//USART_envia(freq);
		//	writeCharacter(freq);
		//	//testeAAAAAAAAAAAAAAAAAAAAA
		//}
	}
}