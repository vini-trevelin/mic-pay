#define TECLA_INVALIDA  25
#define DEBOUNCE 10
#define KEY_1 0
#define KEY_2 1
#define KEY_3 2
#define KEY_4 3
#define KEY_5 4
#define KEY_6 5
#define KEY_7 6
#define KEY_8 7
#define KEY_9 8
#define KEY_APAGAR 9
#define KEY_0 10
#define KEY_CONFIRMA 11
#define KEY_CONFIRMA_ASCII 60
#define KEY_APAGAR_ASCII 58
#define TECLA_INVALIDA_ASCII  123
#define KEY_0_ASCII  59


uint8_t tecla_lida();
uint8_t teclaDebouce();
char acao_tecla(uint8_t num);
void setupBotoes();



