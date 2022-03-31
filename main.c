
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
#include "Modulos/vendas.c"
#include "Modulos/senha.c"
#include "Modulos/telas.c"

#include "Headers/botoes.h"
#include "Headers/senha.h"

#include <util/delay.h>

#define N_CARACSENHA 6 // todas as senhas com 4 caracteres? REVER ISSO



//////////////////////////////////////////////////////////////////////////
//GLOBAIS//
//////////////////////////////////////////////////////////////////////////


char teclaG = TECLA_INVALIDA;
char contTelaOnOff = 0; //para segurar o botão por 3 segundos
char telaOnOf = 0; //para saber o estado da tela (inicia Desligada)

 char userIndex= 10;
 short controle = 1;

//////////////////////////////////////////////////////////////////////////
						//INTERUPÇÕES//
//////////////////////////////////////////////////////////////////////////


ISR(TIMER1_COMPA_vect){
	//writeString("."); //fica escrevendo uns pontos na tela pra gente ver +- se ta 1 seg
	if (teclaG == KEY_CONFIRMA)
	contTelaOnOff++;
	else
	contTelaOnOff = 0;
	
	if(contTelaOnOff == 3){ // se for # por 3 segundos
		if (telaOnOf){
				writeInstruction(lcd_LineOne | lcd_SetCursor);
				writeInstruction(lcd_DisplayOff); //desliga se tiver ligada
				telaOnOf = 0;
			}else{
				writeInstruction(lcd_LineOne | lcd_SetCursor);
				writeInstruction(lcd_DisplayOn); //liga se tiver desligada
				telaOnOf = 1;
		}
		contTelaOnOff = 0;
	}
}

//////////////////////////////////////////////////////////////////////////
//FUNÇÔES//
//////////////////////////////////////////////////////////////////////////


void setTimer1_UmSeg(){
	// 16 MHz -> 1 instrução = 62.5ns
	// Pre scaler de 256 -> 62500Hz logo 62500 contagens para 1 seg

	TCCR1A = 4; //modo comparação
	TCCR1B = 0x4; //pre scaler de 256
	TCNT1 = 0;  //contagem começa do zero
	OCR1A = 62500; //até onde tem q contar
	TIMSK1 |= (1<<OCIE1A); //habilita interrupção por comparação em 
	sei(); // talvez colocar no mais ??
}

int main(){
	
	USART_INIT(UBRR);
	setup_lcd();
	setupBotoes();
	setTimer1_UmSeg();
	//char str[3];	
	
	/*
	Antes do while(1) teremos um setupMaquina();
		Limpa e desliga a tela
		vamos precisar de uma varivael global bool para ver se a tela esta on ou off
		
		
	Talvez para a primeira  ligada não precisaria fazer o esquema com timer
	só deixar um 
	while(flag){
		if(tecla_lida == #) 
			flag = 0;	
		_delay_ms(2000); //ou 3000, não lembro o que ele pediu
		} 
	e dai dps vem o while grande q vai controlar tudo
	
	*/
	
	//rotina para ligar 1° vez
	// ...
	
	writeInstruction(lcd_DisplayOn);
	writeString("Bloqueado"); //ficar mais fiel ao pdf
	_delay_ms(2000);
	writeInstruction(lcd_Home | lcd_Clear);
	
	while(1){
		//while (!telaOnOf){// lcd incia desligado e fico lendo a tecla caso # por 3s mais liga o LCD
		//	teclaG = teclaDebouce();
		//}
		//writeInstruction(lcd_DisplayOn); // comentar quando colocar a rotina de ligar a tela
		teclaG =TECLA_INVALIDA;
		userIndex = login(&teclaG); //o que faz tudo com relação a senha inicial, retorna quem entrou (0 adm, 1 e 2 operadores 1 e 2)
		
		if(userIndex != 0){
			while(1){
				writeInstruction(lcd_Home | lcd_Clear);
				menu_vendas();
				while(teclaG+49 > 51){
					teclaG = teclaDebouce();
				}
				processa_venda(teclaG+49);	// para testar cartão por aprox coloca CW + 6dig docartao + 6 dig da senha no serial
				break;
			}
		}
		
		if(userIndex == 0){
			// MODO ADM....
		}
		
		//while(1); // só para parar aqui
	}
	
}
	
	
	/*
		sprintf(sfinal,"%d",i); //codigozinho pra escrever as variaves no lcd
		writeString(s2);
		writeInstruction(lcd_LineTwo | lcd_SetCursor);
		writeString(sfinal);
		*/
	
	