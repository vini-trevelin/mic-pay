#include <avr/io.h>
#include <util/delay.h>

void write(uint8_t);
void writeInstruction(uint8_t);
void writeCharacter(uint8_t);
void writeString(char *);
void lcd_init_4d(void);