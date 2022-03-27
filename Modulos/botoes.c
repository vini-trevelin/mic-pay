
//////////////////////////////////////////////////////////////////////////
						//INCLUDES//
//////////////////////////////////////////////////////////////////////////


#include <avr/io.h>
#include <avr/interrupt.h>
#include "../Headers/botoes.h"
#include "../Headers/delayT1.h"
#include "../Headers/lcd.h"
#include <util/delay.h>
#define TECLA_INVALIDA  25
#define DEBOUNCE 10
void setupBotoes(){
	//Não faz nada pq por alguma motivo o DDRD e DDRC tem q ficar dentro do tecla_lida()
}


uint8_t tecla_lida(){
	uint8_t linha,coluna;
	
	DDRC &=~(0x0F);
	DDRD &=~(0x70);

	PORTC |= 0x0F;					// coloca todas linhas em 1

	for(coluna=0;coluna<3;coluna++){
		DDRD|=(0x10<<coluna);					// define denovo? apenas uma entrada por vez (coluna)
												// 0100 0000 >> c mudei para 0001 0000 << c pois minha coluna 1 eh PD4
		_delay_us(50);
		for(linha=0;linha<4;linha++){
			if(!(PINC & (0x01<<linha))){		// 0000 1000 (verifica PINC deslocado do numero de linhas)
				return (linha*3+coluna);
			}
		}
	}
	return TECLA_INVALIDA;
}


uint8_t teclaDebouce(){
	uint8_t teclaAnterior, teclaAtual;
	teclaAnterior = tecla_lida();
	char contador=0;
	while(contador < DEBOUNCE){
		_delay_ms(4);
		teclaAtual = tecla_lida();
		if(teclaAtual == teclaAnterior){
			contador++;
		}
		else{
			contador =0;
		}
		teclaAnterior = teclaAtual;
	}
	return teclaAtual;
}
   //	 C  	C    C
   //L	|  0 |  1 |  2 |  		|  1 |  2 |  3 |
   //L	|  3 |  4 |  5 |   =>	|  4 |  5 |  6 |
   //L	|  6 |  7 |  8 | 		|  7 |  8 |  9 |
   //L	|  9 | 10 | 11 | 		|  * |  0 |  # |

char acao_tecla(uint8_t num){ // tirar o writeString daqui quando formos usar de vdd, coloquei aqui só pra mostrar a leitura no display
	switch (num){
		case 0:
			//writeString("1");
			return 1;
		break;
		
		case 1:
			//writeString("2");
			return 2;
		break;
		
		case 2:
			//writeString("3");
			return 3;
		break;
		
		case 3:
			//writeString("4");
			return 4;
		break;
		
		case 4:
			//writeString("5");
			return 5;
		break;
		
		case 5:
			//writeString("6");
			return 6;
		break;
		
		case 6:
			//writeString("7");
			return 7;
		break;
		
		case 7:
			//writeString("8");
			return 8;
		break;
		
		case 8:
			//writeString("9");
			return 9;
		break;
		
		case 9:
			//writeString("*");
			return 10;
		break;
		
		case 10:
			//writeString("0");
			return 11;
		break;
		
		case 11:
			//writeString("#"); 
			return 12;
		break;
		
		default:
			return TECLA_INVALIDA;
		break;
	}
}

/*Por ter começado a funcionar 
	Obrigado, Pai, por Teu perdão e por dar-me uma vida plena e abundante. Senhor, a Ti, que
	és o Criador de tudo o que sou e do que possuo, dedico a minha vida, clamando para que
	eu veja e faça sempre a Tua vontade, e que minhas obras honrem e glorifiquem o Teu
	Nome. Amém!”
*/


