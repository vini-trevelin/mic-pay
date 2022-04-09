
//////////////////////////////////////////////////////////////////////////
						//INCLUDES//
//////////////////////////////////////////////////////////////////////////

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Headers/botoes.h"
#include "Headers/senha.h"
#include "Headers/cartoes.h"
#include "Headers/lcd.h"
#include "Headers/serial.h"
#include "Headers/relogio.h"
#include "Headers/pendencias.h"

#include <util/delay.h>

#define N_CARACSENHA 4				// todas as senhas com 4 caracteres? s, menos de cartão
#define TEMPO_COMUNICACAO 120		// 2min   (tempo para verificação da comunicação externa)
#define TEMPO_REQUISICAO 13			// 13s (tempo para verificação de resposta a requisição ext)

//////////////////////////////////////////////////////////////////////////
//GLOBAIS//
//////////////////////////////////////////////////////////////////////////


char teclaG = TECLA_INVALIDA;
char contTelaOn = 0;							//para segurar o botão por 3 segundos
char contTelaOff = 0;
char telaOnOff = 0;								//para saber o estado da tela (inicia Desligada)
static char contSerial = 0;						//para saber quanto tempo a serial esta sem comunicação
static char com_ext = 0;						//para saber se comunicação ext foi requisitada (vendas.c)

char userIndex= 10;
short controle = 1;
char flagRELOGIO = 0;


//static para manterem os valores durante as incializações
static char numCartoesLocais [LOCALCARDNUM][CARDSIZE]; // indices correspondentes entre os 3  elementos
static char senhasLocais[LOCALCARDNUM][SENHASIZE];
static char cashBackCounter[LOCALCARDNUM];
static char saldosLocais[LOCALCARDNUM][DIGITOS]; // armazenados na forma mais significativo -> indice 0;
unsigned static char cartoesCadastrados; // inicia sem nenhum cadastro;

//////////////////////////////////////////////////////////////////////////
//INCLUDES MÓDULOS//
//////////////////////////////////////////////////////////////////////////

#include "Modulos/relogio.c"
#include "Modulos/cartoes.c"
#include "Modulos/vendas.c"
#include "Modulos/botoes.c"
#include "Modulos/delayT1.c"
#include "Modulos/serial.c"
#include "Modulos/lcd.c"
#include "Modulos/senha.c"
#include "Modulos/telas.c"
#include "Modulos/modoAdm.c"
#include "Modulos/pendencias.c"

//////////////////////////////////////////////////////////////////////////
						//INTERUPÇÕES//
//////////////////////////////////////////////////////////////////////////


ISR(TIMER1_COMPA_vect){									// interrupção cada 1 segundo
	updateDate();
	
	if(flagRELOGIO)
		tela_dataAtual(); //para colocar relogio na tela (modo adm)
	
	contSerial = verificaSerial(contSerial); //verifica comunicacao

	if(contSerial==TEMPO_COMUNICACAO){					// segundos para aguardar comunicação cm serial
		contSerial = 0;
		PORTC ^= (1 << PORTC4);
	}
	
	if(com_ext != 0){
		com_ext++;
	}

	if (teclaG == KEY_CONFIRMA) //contador de ligar tela
		contTelaOn++;
	else
		contTelaOn = 0;
	
	if (teclaG == KEY_APAGAR) //contador de desligar tela
		contTelaOff++;
	else
		contTelaOff = 0;
	
	if(contTelaOn == 3 && telaOnOff==0){ // se for # por 3 segundos e tiver desligada -> liga tela
		writeInstruction(lcd_DisplayOn);
		contTelaOn = 0;
		telaOnOff = 1;
		
	}
	if (contTelaOff == 4 && telaOnOff==1){ //se * por 4 segs e tiver ligada -> desliga
		writeInstruction(lcd_DisplayOff);
		contTelaOff = 0;
		telaOnOff = 0;
	}
					
	
	if((horas == 12 || horas == 18 || horas == 22) && minutos==0 && segundos == 0) { //verificacao de pags agendados e pendentes
		cobrarPagementosAgendados();
		if (horas == 22)
			verificarPendencia();
	}
	
	if(existePendencia()){ //pisca led caso pendencia 
		PORTC ^= (1<<5);
	}else{
		PORTC &= ~(1<<5);
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
	
	anoZero();							// inicia o relogio
	USART_INIT(UBRR);					// inicia serial
	setup_lcd();						// inicia lcd
	setupBotoes();						// inicia teclado
	setTimer1_UmSeg();					
	char loop1 = 1; //para ele agir diferente no 1 loop
	char continuar;

// 	writeInstruction(lcd_DisplayOff);
// 	while(telaOnOff==0)
// 		teclaG = teclaDebouce();

	writeInstruction(lcd_DisplayOn);
	tela_bloqueio_inicial(); //comenta se quiser testar mais rapido
	
	
	// inserindo cartões pra teste
	char cartao_teste[] = {8+48,0+48,0+48,0+48,0+48,0+48};
	char cartao_teste_2[] = {0+48,0+48,2+48,0+48,0+48,0+48};
	addCartao("123456","123456",cartao_teste);	// inserindo cartão pra teste 80000 (800,00)
	addCartao("000001","111111",cartao_teste_2);	// inserindo cartão pra teste 00200	(2,00)
	/////
	
	while(1){
		
		if(loop1){
			userIndex = login(&teclaG); //o que faz tudo com relação a senha inicial, retorna quem entrou (0 adm, 1 e 2 operadores 1 e 2)
			loop1 = 0;
		}else{
			continuar = continuar_modo_atual(&teclaG);
			if(!continuar)
				userIndex = login(&teclaG); //pode mudar o modo adm <-> oper
		}
		
		teclaG = TECLA_INVALIDA;
		if(userIndex != 0){ // se é um operador
			if(getOperStatus(userIndex))
				while(1){
					writeInstruction(lcd_Home | lcd_Clear);
					menu_vendas();
					while(teclaG+49 > 51){
						teclaG = teclaDebouce();
					}
					processa_venda(teclaG+49);	// para testar cartão por aprox coloca CW + 6dig docartao + 6 dig da senha no serial
					break;
				}
			else{
				tela_operadorDesabilitado(userIndex);
				}
		}
		
		if(userIndex == 0){ //se é o adm
			menuADM();
			teclaG = TECLA_INVALIDA;
			while(teclaG == TECLA_INVALIDA){
				teclaG = teclaDebouce();
			}
			modoADM(&teclaG);
		}
	}
	
}
