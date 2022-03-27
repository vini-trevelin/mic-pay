
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

char teclaG = 20; //fazer ser = TECLA_INVALIDA dps
char contTelaOnOff = 0; //para segurar o botão por 3 segundos
char telaOnOf = 1; //para saber o estado da tela
//#define TESTE 1 //comentar para não fazer o main


//////////////////////////////////////////////////////////////////////////
						//INTERUPÇÕES//
//////////////////////////////////////////////////////////////////////////


// #ifdef TESTE 
// 	ISR(TIMER0_OVF_vect){						// Interrupcao por overflow do TIMER0
// 		if(freq!=TECLA_INVALIDA){
// 			TCNT0 =freq*20;
// 			PORTB ^= (1 << 5);
// 			TIFR0 |= (1<<0);						//limpa estouro
// 		}
// 	}
// #endif

void setTimer1_UmSeg(){
	// 16 MHz -> 1 instrução = 62.5ns
	// Pre scaler de 256 -> 62500Hz logo 62500 contagens para 1 seg

	TCCR1A = 4; //modo comparação
	TCCR1B = 0x4; //pre scaler de 256

	TCNT1 = 0;  //contagem começa do zero
	OCR1A = 62500; //até onde tem q contar
	TIMSK1 |= (1<<OCIE1A); //habilita interrupção por comparação em A

	sei();
}

ISR(TIMER1_COMPA_vect){
	writeString("."); //fica escrevendo uns pontos na tela pra gente ver +- se ta 1 seg
	if (teclaG == 12)
		contTelaOnOff++;
	else
		contTelaOnOff = 0;
		
	if(contTelaOnOff == 3){ // se for # por 3 segundos
		if (telaOnOf){
			writeInstruction(lcd_DisplayOff); //desliga se tiver ligada 
			telaOnOf = 0;
		}else{
			writeInstruction(lcd_DisplayOn); //liga se tiver desligada
			telaOnOf = 1;
		}
		contTelaOnOff = 0;
	}
}


int main(){
	
	//USART_INIT(UBRR);
	setup_lcd();
	setupBotoes();
	setTimer1_UmSeg();
	//char str[3];	
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
		
		teclaG = acao_tecla(teclaDebouce());
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