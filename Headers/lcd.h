#define F_CPU 16000000UL // util/delay.h usa

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

void write(uint8_t);
void writeInstruction(uint8_t);
void writeCharacter(uint8_t);
void writeString(char *);
void setup_lcd(void);