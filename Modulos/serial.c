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

// Fun��o para enviar 
void USART_envia(unsigned char dados){
	while(!(UCSR0A & (1<<UDRE0)));							// espera limpeza da transmiss�o (registrador)
	UDR0 = dados;											// coloca dado a ser enviado no registrador
}

// Fun��o para receber 
unsigned char USART_recebe(void){
	while(!(UCSR0A & (1<<RXC0)));							// espera algum dado ser recebido (registrador)
	return UDR0;											// retorna dado recebido
}
char verificaSerial(short contSerial){
	

	if(!(UCSR0A & (1<<RXC0))){
		contSerial++;
	}else if(UDR0){
		contSerial=0;
		PORTC &= ~(1 << PORTC4);
	}		
	return contSerial;								
}