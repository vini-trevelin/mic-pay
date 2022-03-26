
#include <util/delay.h>
//bit de D7 .. D4 é PORTD7 .. PORTD4
#define lcdPort PORTB
#define lcd_E_bit       PORTB1
#define lcd_RS_bit      PORTB0 //RS = regsitrer Select =>  1 p/ receber info (texto), 0 p/ configurar o lcd

#include "../Headers/lcd.h"

void setup_lcd(){
	
	//configura os pinos da porta B para saida
	//f3 = 1111 0011 = B7 B6 ... B1 B0
	DDRB = 0xf3;
	
    lcdPort &= ~(1<<lcd_RS_bit);                //RS = 0 -> setar parametros de config
    lcdPort &= ~(1<<lcd_E_bit);                  // enable 0

	//Reset
    write(lcd_FunctionReset);
    _delay_ms(10);
 
    write(lcd_FunctionSet4bit);
    _delay_us(40);                                

	// Coloca modo 4 bits
    writeInstruction(lcd_FunctionSet4bit);

	//Display Off
    writeInstruction(lcd_DisplayOff); 
	
	//Limpa display
    writeInstruction(lcd_Clear);
	
	//modo para escrita
    writeInstruction(lcd_EntryMode);
    
	// Display On
    writeInstruction(lcd_DisplayOn); 
}

void writeString(char text[])
{
    volatile int i = 0;                            
    while (text[i] != 0)
    {
        writeCharacter(text[i]);
        i++;
        _delay_us(40);     
    }
}


void writeCharacter(uint8_t caracter)
{
    lcdPort |= (1<<lcd_RS_bit);                // garante RS = 0
    lcdPort &= ~(1<<lcd_E_bit);                
    write(caracter);                            // primeiros 4 bits
    write(caracter << 4);                      // ultimos 4 bits
}


void writeInstruction(uint8_t instruction)
{
    lcdPort &= ~(1<<lcd_RS_bit);                // garante RS = 0
    lcdPort &= ~(1<<lcd_E_bit);                
    write(instruction);                    // primeiros 4 bits
    write(instruction << 4);               // ultimos 4 bits
	 _delay_us(40); 
}


void write(uint8_t bitCmd)
{
	//coloca zero na portaD
    lcdPort &= ~(1<<PORTB7);
	lcdPort &= ~(1<<PORTB6);
	lcdPort &= ~(1<<PORTB5);
	lcdPort &= ~(1<<PORTB4);
	             
	//se tem 1 no bitRecebido na posição de D7 .. D4 dai coloca 1 lá          
    if (bitCmd & 1<<7) 
		lcdPort |= (1<<PORTB7);    
                        
    if (bitCmd & 1<<6) 
		lcdPort |= (1<<PORTB6);

    if (bitCmd & 1<<5) 
		lcdPort |= (1<<PORTB5);

    if (bitCmd & 1<<4) 
		lcdPort |= (1<<PORTB4);
		
    lcdPort |= (1<<lcd_E_bit);                   // Enable 1 (escreve)
    _delay_us(1);                                  
    lcdPort &= ~(1<<lcd_E_bit);                  // Enable 0 
    _delay_us(1);                                   
}