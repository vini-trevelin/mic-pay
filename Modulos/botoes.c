
//////////////////////////////////////////////////////////////////////////
						//INCLUDES//
//////////////////////////////////////////////////////////////////////////


#include <avr/io.h>
#include <avr/interrupt.h>
#include "../Headers/botoes.h"
#include "../Headers/delayT1.h"
#define TECLA_INVALIDA  20
//#define TESTE 1 //comentar para não fazer o main



//////////////////////////////////////////////////////////////////////////
						//GLOBAIS//
//////////////////////////////////////////////////////////////////////////


char CICLOSDEBOUNCE = 7;
char freq = TECLA_INVALIDA;								// inicia sem gerar onda

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

//////////////////////////////////////////////////////////////////////////
						//FUNÇÕES//
//////////////////////////////////////////////////////////////////////////

void setupTeclado(){
	//PORTAS
	DDRD = 0x8F;							// Para Teclado PD4 - PD6  como entrada
	DDRC = 0x3F;							// Define porta C como saída todos os bits
	PORTC = 0X3F;							// Liga os bits da porta C (Já desliga os leds também)
	//TIMERS
	TCCR0A = 0;								// configura modo normal timer0
	TCCR0B = 0x4;							// configura preescaller parao timer0 clock/256 -> 16 microssegundos
}


void enableTestTeclado(){
	DDRB |= (1<<5);							// Define porta B como saída
	PORTB  |= (1 << 5);
}

void testaTeclado(){
	char teclaLida = varrerTeclado();
	if (teclaLida!=TECLA_INVALIDA){
		execCmdTecla(teclaLida);
	}
}

void execCmdTecla(char t){ // TALVEZ : Retirar essa função daqui e colocar na parte de código de lógica mesmo 
	switch (t){ // inserir o que cada tecla faz no momento não estou usando pois só estou atribuindo o valor de t para freq;
		case  0: // tecla 1
			freq =  t;
			break;
		case  1: // tecla 2
			freq =  t;
			break;
		case  2: // tecla 3
			freq =  t;
			break;
		case  3: // tecla 4
			freq =  t;
			break;
		case  4: // tecla 5
			freq =  t;
			break;
		case  5: // tecla 6
			freq =  t;
			break;
		case  6: // tecla 7;
			freq =  t;
			break;
		case  7: // tecla 8;
			freq =  t;
			break; 
		case  8: // tecla 9
			freq =  t;
			break;
		case  9: // tecla *
			freq =  t;
			break;
		case 10: // tecla 0
			freq =  t;
			break;
		case 11: // tecla #
			freq =  t;
			break;
		default: // tecla invalida 
			break;
	}

}

char varrerTeclado(){
	/*
	teclado 
	-Representação Interna		Correnspondência Real
	COLUNAS = PD4-PC6  //saídas
	LINHAS  = PC0-PC3  //Entradas
		   C	C    C
	  L	|  0 |  1 |  2 |  		|  1 |  2 |  3 | 
	  L	|  3 |  4 |  5 |   =>	|  4 |  5 |  6 | 
	  L	|  6 |  7 |  8 | 		|  7 |  8 |  9 | 
	  L	|  9 | 10 | 11 | 		|  * |  0 |  # | 
	*/
	char i,j,notPressionado,tecla,flag;
	flag=0;										// indica status da leitura 
	tecla = TECLA_INVALIDA ;					// leitura invalida
	for (i =0; i<4;i ++){						// i = linhas
		PORTC = ~ (1 << i);						// Desliga uma linha
		for (j = 0; j < 3; j++){				// le uma coluna
			notPressionado = debounce(j);
			if (!notPressionado){
				tecla = 3*i+j;
				flag=1;
			}
			if (flag==1){	
				break;
			}
		}
		if (flag==1){	
				break;
			}
	}
	return tecla;
}


char debounce(char pino){					//ROTINA PARA DEBOUNCING
	char cont, key_last, key_now;
	cont=0;
	key_last=0;
	key_now=0;
	pino = pino + 4;
	while(cont != CICLOSDEBOUNCE){
		key_last = key_now;
		delay();							//1ms
		key_now = PIND&(1<<pino);			//LÊ pino
		if(key_now == key_last){
			cont++;
			}else{
			cont=0;
		}
	}
	return key_now;							//RETORNA se foi pressionado (0) ou não (1)
	// Considerei, que uma leitura em alto implica conexão aberta
}

#ifdef TESTE
// para teste do arquivo em semparado
int main(){
	char teclaLida =  TECLA_INVALIDA;		// inicia como uma tecla invalida
	setupTeclado();							// inicializa o teclado
	enableDelayT1();							// para usar o timer1 para delay-> Temos que ver se não vamos usar aquela função pronta dai liberariamos esse timer
	enableTestTeclado();
	while (1)
	{
		teclaLida = varrerTeclado();
		if (teclaLida!=TECLA_INVALIDA){
			execCmdTecla(teclaLida);
		}
	}
	
}
#endif