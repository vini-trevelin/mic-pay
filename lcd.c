#define F_CPU 16000000UL // util/delay.h usa

//bit de D7 .. D4 é PORTD7 .. PORTD4
#define lcdPort PORTB
#define lcd_E_bit       PORTB1
#define lcd_RS_bit      PORTB0 //RS = regsitrer Select =>  1 p/ receber info (texto), 0 p/ configurar o lcd

#define lcd_LineOne     0x00                    // começo linha 1
#define lcd_LineTwo     0x40                    // começo linha 2

// LCD comandos
#define lcd_Clear           0b00000001          // limpa o lcd
#define lcd_Home            0b00000010          // coloca cursos na posição 0
#define lcd_EntryMode       0b00000110          // modo de escrita da entrada (anda da esq para dir)
#define lcd_DisplayOff      0b00001000          // display off
#define lcd_DisplayOn       0b00001100          // display on, cursor off
#define lcd_FunctionReset   0b00110000          // reseta LCD
#define lcd_FunctionSet4bit 0b00101000          // 4-bit
#define lcd_SetCursor       0b10000000          // Pra setar posição

void lcd_init_4d(){
	
	//configura os pinos da porta D para saida
	//f3 = 1111 0011 = D7 D6 ... D1 D0
	DDRB = 0xf3;
	
    lcdPort &= ~(1<<lcd_RS_bit);                //RS = 0 -> setar parametros de config
    lcdPort &= ~(1<<lcd_E_bit);                  // enable 0

	//Reset
    write(lcd_FunctionReset);
    _delay_ms(10);
 
    write(lcd_FunctionSet4bit);
    _delay_us(80);                                

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
        _delay_us(80);     
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
	 _delay_us(80); 
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