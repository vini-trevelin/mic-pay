
//////////////////////////////////////////////////////////////////////////
						//INCLUDES//
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Modulos/botoes.c"
#include "Modulos/delayT1.c"
#include "Modulos/serial.c"
#include "Modulos/lcd.c"
#include <util/delay.h>

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
	setup_lcd();
	setupBotoes();
	
	//char teclaLida =  TECLA_INVALIDA;			// inicia como uma tecla invalida
	
	/*
	Antes do while(1) teremos um setupMaquina();
		Limpa e desliga a tela
		
	Como pretendo fazer a tela ligar/desligar:
		Se a tecla lida for = #:
			Liga um timer e interrupçoes 
			Deixa o prog rodando
			dps que passar os N segundos la
			timer da OV e ve se a tecla ainda é #
			se for desliga/liga
			(poderia ate desligar o timer dps, mas provavelmente vamos usar ele para mais coisa)
			(podemos fazer nossa padrão ser o segundo e dai só fazemos multiplos dele)
		
		Para isso vamos precisar de uma varivael global tecla_lida e de uma bool para ver se esta on ou off
		
		
	Talvez para a primeira  ligada não precisaria fazer o esquema com timer
	só deixar um 
	while(flag){
		if(tecla_lida == #) 
			flag = 0;	
		_delay_ms(2000); //ou 3000, não lembro o que ele pediu
		} 
	e dai dps vem o while grande q vai controlar tudo
	
	No momento acao_tecla (antiga valor_tecla) não retorna nada
	mas provavelmente terá q retornar para que possamos usar tecla_lida aqui na main
	
	Implementar debounce (e torcer para continuar funcionando)
	*/
	
	
	while (1){
		
		acao_tecla(tecla_lida());
		_delay_ms(50);
		
	}
}

	//teclaLida = varrerTeclado();
	//if (teclaLida!=TECLA_INVALIDA){
	//	execCmdTecla(teclaLida);
	//	//USART_envia(freq);
	//	writeCharacter(freq);
	//	//testeAAAAAAAAAAAAAAAAAAAAA
	//}
	
	
	/*
		sprintf(sfinal,"%d",i); //codigozinho pra escrever as variaves no lcd
		writeString(s2);
		writeInstruction(lcd_LineTwo | lcd_SetCursor);
		writeString(sfinal);
		*/