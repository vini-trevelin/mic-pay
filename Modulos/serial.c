//#define F_CPU 16000000	//freq CPU (16Mhz) JA ESTOU DEFININDO NO LCD
#define BAUD 19200		//baud rate
#define UBRR F_CPU/16/BAUD-1
#include "../Headers/serial.h"

// Inicializa��o de USART

void USART_INIT(unsigned int ubrr){
	UBRR0H = (unsigned char)(ubrr >> 8);					//registrador taxa de transmiss�o parte alta
	UBRR0L = (unsigned char)ubrr;							//registrador taxa de transmiss�o parte baixa
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);							//habilito transmissor e receptor
	UCSR0C = (1<<USBS0)|(UCSZ00);							
}

// Fun��o para enviar  dados a serial
void USART_envia(unsigned char dados){
	while(!(UCSR0A & (1<<UDRE0)));							// espera limpeza da transmiss�o (registrador)
	UDR0 = dados;											// coloca dado a ser enviado no registrador
}

// Fun��o para receber dados da serial
unsigned char USART_recebe(void){
	while(!(UCSR0A & (1<<RXC0))){							// espera algum dado ser recebido (registrador)
		switch(com_ext){									// trato a flag de comunica��o externa
			case 2: tela_processandoVenda();break;
			case 6: tela_processandoVenda2();break;
			case 10: tela_processandoVenda3();break;
			// fica meio bugado pq fica sobrescrevendo tela mas efeito ficou legal ent�o deixei
			case TEMPO_REQUISICAO: tela_erroCom();com_ext = 0;return 0;// indica que n�o recebi algo no registrador, zero comunica��o externa
		}
	}						
	return UDR0;											// retorna dado recebido
}
char verificaSerial(short contSerial){

	if(!(UCSR0A & (1<<RXC0))){			// caso n�o exista nada no registrador
		contSerial++;					// somo +1
	}else if(UDR0){						// se existir algo, quer dizer que houve comunica��o externa
		contSerial=0;					// zero o meu contador
		PORTC &= ~(1 << PORTC4);		// coloco 0 no meu pino C
	}		
	return contSerial;								
}