
//////////////////////////////////////////////////////////////////////////
						//INCLUDES//
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "Headers/botoes.h"
#include "Headers/senha.h"
#include "Headers/cartoes.h"
#include "Headers/lcd.h"
#include "Headers/serial.h"
#include "Headers/relogio.h"

#include <util/delay.h>

#define N_CARACSENHA 6 // todas as senhas com 4 caracteres? REVER ISSO

//////////////////////////////////////////////////////////////////////////
//GLOBAIS//
//////////////////////////////////////////////////////////////////////////


char teclaG = TECLA_INVALIDA;
char contTelaOnOff = 0; //para segurar o bot�o por 3 segundos
char telaOnOf = 0; //para saber o estado da tela (inicia Desligada)

char userIndex= 10;
short controle = 1;

//static para manterem os valores durante as incializa��es
static char numCartoesLocais [LOCALCARDNUM][CARDSIZE]; // indices correspondentes entre os 3  elementos
static char senhasLocais[LOCALCARDNUM][SENHASIZE];
static char cashBackCounter[LOCALCARDNUM];
static char saldosLocais[LOCALCARDNUM][DIGITOS]; // armazenados na forma mais significativo -> indice 0;
unsigned static char cartoesCadastrados; // inicia sem nenhum cadastro;

//////////////////////////////////////////////////////////////////////////
//INCLUDES M�DULOS//
//////////////////////////////////////////////////////////////////////////

#include "Modulos/cartoes.c"
#include "Modulos/vendas.c"
#include "Modulos/botoes.c"
#include "Modulos/delayT1.c"
#include "Modulos/serial.c"
#include "Modulos/lcd.c"
#include "Modulos/senha.c"
#include "Modulos/telas.c"
#include "Modulos/modoAdm.c"
#include "Modulos/relogio.c"



//////////////////////////////////////////////////////////////////////////
						//INTERUP��ES//
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
	updateDate();
	AtualizaStringDataHora(); // opcional estariamos disperdi��o processamento uma vez que essa fun��o s� converte a data atual para string
}

//////////////////////////////////////////////////////////////////////////
//FUN��ES//
//////////////////////////////////////////////////////////////////////////

		
		/*void testaRelogio(){
			writeInstruction(lcd_DisplayOn);

			writeString(stringHORA);
			char ArrNovoDia[] ={'0','1'};
			char ArrNovoMes[] ={'0','4'};
			char ArrNovoAno[] = {'2','2'};
			char ArrNovaHora[] = {'1','7'};
			char ArrNovoMinuto[] = {'2','2'};
			char ArrNovoSegundo[] = {'1','0'};
			changeDate( ArrNovoDia, ArrNovoMes, ArrNovoAno,  ArrNovaHora, ArrNovoMinuto, ArrNovoSegundo);
			while (1){
			writeInstruction(lcd_LineOne | lcd_SetCursor);
			writeString(stringDATA);
			writeInstruction(lcd_LineTwo | lcd_SetCursor);
			writeString(stringHORA);
			}
		}
		*/


void setTimer1_UmSeg(){
	// 16 MHz -> 1 instru��o = 62.5ns
	// Pre scaler de 256 -> 62500Hz logo 62500 contagens para 1 seg
	
	TCCR1A = 4; //modo compara��o
	TCCR1B = 0x4; //pre scaler de 256
	TCNT1 = 0;  //contagem come�a do zero
	OCR1A = 62500; //at� onde tem q contar
	TIMSK1 |= (1<<OCIE1A); //habilita interrup��o por compara��o em 
	sei(); // talvez colocar no mais ??
}

int main(){
	anoZero(); // inicia o relogio
	USART_INIT(UBRR);
	setup_lcd();
	setupBotoes();
	setTimer1_UmSeg();
	char loop1 = 1; //para ele agir diferente no 1 loop
	char continuar;
	//char str[3];	
	//testaRelogio();
	/*
	Antes do while(1) teremos um setupMaquina();
		Limpa e desliga a tela
		vamos precisar de uma varivael global bool para ver se a tela esta on ou off
		
		
	Talvez para a primeira  ligada n�o precisaria fazer o esquema com timer
	s� deixar um 
	while(flag){
		if(tecla_lida == #) 
			flag = 0;	
		_delay_ms(2000); //ou 3000, n�o lembro o que ele pediu
		} 
	e dai dps vem o while grande q vai controlar tudo
	
	*/
	
	//rotina para ligar 1� vez
	// ...
	
	writeInstruction(lcd_DisplayOn);
	tela_bloqueio_inicial(); //comenta se quiser testar mais rapido
	
	
	// inserindo cart�es pra teste
	char cartao_teste[] = {8+48,0+48,0+48,0+48,0+48,0+48};
	char cartao_teste_2[] = {0+48,0+48,2+48,0+48,0+48,0+48};
	addCartao("123456","123456",cartao_teste);	// inserindo cart�o pra teste 80000 (800,00)
	addCartao("000001","111111",cartao_teste_2);	// inserindo cart�o pra teste 00200	(2,00)
	/////
	
	
	while(1){
		//writeInstruction(lcd_DisplayOn); // comentar quando colocar a rotina de ligar a tela
		if(loop1){
			userIndex = login(&teclaG); //o que faz tudo com rela��o a senha inicial, retorna quem entrou (0 adm, 1 e 2 operadores 1 e 2)
			loop1 = 0;
		}else{
			continuar = continuar_modo_atual(&teclaG);
			if(!continuar)
				userIndex = login(&teclaG); //pode mudar o modo adm <-> oper
		}
		teclaG = TECLA_INVALIDA;
		if(userIndex != 0){
			while(1){
				writeInstruction(lcd_Home | lcd_Clear);
				menu_vendas();
				while(teclaG+49 > 51){
					teclaG = teclaDebouce();
				}
				processa_venda(teclaG+49);	// para testar cart�o por aprox coloca CW + 6dig docartao + 6 dig da senha no serial
				break;
			}
		}
		
		if(userIndex == 0){
			menuADM();
			teclaG = TECLA_INVALIDA;
			while(teclaG == TECLA_INVALIDA){
				teclaG = teclaDebouce();
			}
			modoADM(&teclaG);
		}
	}
	
}
