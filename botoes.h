/*
 * botoes.h
 *
 * Created: 12/03/2022 11:57:09
 *  Author: VITOR
 */ 

void execCmdTecla(char tecla); // executa o comando de uma tecla ()
char varrerTeclado();		   // varre o teclado
char debounce(char pino);	   // debounce (usar o timer 1 )
void setupTeclado();		   // Setup do teclado
void enableTestTeclado();	   // Habilita o teste no teclado no led interno (controlando a frquencia de uma osiclação)
#define TECLA_INVALIDA  20
