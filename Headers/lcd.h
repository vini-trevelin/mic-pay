#define F_CPU 16000000UL // util/delay.h usa
void write(uint8_t);
void writeInstruction(uint8_t);
void writeCharacter(uint8_t);
void writeString(char *);
void lcd_init_4d(void);