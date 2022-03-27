
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
#include "Headers/botoes.h"
#include <util/delay.h>

#define N_CARACSENHA 4 // todas as senhas com 4 caracteres? REVER ISSO



//////////////////////////////////////////////////////////////////////////
//GLOBAIS//
//////////////////////////////////////////////////////////////////////////


char teclaG = TECLA_INVALIDA;
char contTelaOnOff = 0; //para segurar o botão por 3 segundos
char telaOnOf = 0; //para saber o estado da tela (inicia Desligada)
int SENHAADM   = 1234;
int SENHAUSER1 = 0001;
int SENHAUSER2 = 0002;

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

int lerSenha(){ // comecei a pensar no ler senha e tals. Não sei se passar tudo para inteiro é a melhor forma de montar a senha
	//também tem questão dos numeros de digitos na senha e a parte de apagar o caractere que não está funcionando 100% (não estou limpando onde estou pagando no display)
	char teclaG = acao_tecla(teclaDebouce());
	char contador = 1;
	int senhaLida =0;
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	char teclaAnterior=0;
	while (teclaG != KEY_CONFIRMA || contador<= N_CARACSENHA){
		teclaG = teclaDebouce();
		if(teclaG!= TECLA_INVALIDA && teclaG!=KEY_CONFIRMA && teclaG !=KEY_APAGAR){
			if(teclaG!= KEY_0){
				switch(contador){
					case 1:
					senhaLida+= (teclaG+1)*1000;
					break;
					case 2:
					senhaLida+= (teclaG+1)*100;
					break;
					case 3:
					senhaLida+= (teclaG+1)*10;
					break;
					case 4:
					senhaLida+= (teclaG+1)*1;
					break;
					default:
					senhaLida+=0;
					break;
				}
			}
			contador++;
			teclaAnterior =teclaG;
			writeString("*");
		}
		else if(teclaG == KEY_APAGAR){
			if(contador>1){
				contador --;
				switch(contador){
					case 1:
					senhaLida-= (teclaAnterior+1)*1000;
					break;
					case 2:
					senhaLida-= (teclaAnterior+1)*100;
					break;
					case 3:
					senhaLida-= (teclaAnterior+1)*10;
					break;
					case 4:
					senhaLida-= (teclaAnterior+1)*1;
					default:
					senhaLida-=0;
					break;
				}
			}
		}
	}
	//writeString("SenhaLida");
	return senhaLida;
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
	
	
	while (!telaOnOf){// lcd incia desligado e fico lendo a tecla caso # por 3s mais liga o LCD
		teclaG = teclaDebouce();
	}
	teclaG =TECLA_INVALIDA;
	writeString("Insira a Senha"); //chamada de senha inicial
	char senhaValida = 0;
	int senhaLida = 0;
	while(!senhaValida){
		senhaLida= lerSenha();
		if (senhaLida == SENHAADM){
			senhaValida =1;
			writeString("bem vindo Adm");
		}
		else if(senhaLida == SENHAUSER1 || senhaLida ==SENHAUSER2){
			senhaValida =1;
			writeString("bem vindo USER");
		}
	}
	//while(1){
		//pensar o resto aqui
		
	//}
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