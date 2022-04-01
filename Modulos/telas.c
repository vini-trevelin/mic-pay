#include <stdio.h>
#include <math.h>
#include "../Headers/telas.h"

void menu_vendas(){
	writeInstruction(lcd_Clear);	
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("[1] AV [2] AP");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("[3] Estorno");
	
	// Opção que fica passando de tela pra outra, precisaria de interrupt pra funcionar 100%
	//_delay_ms(1000);
	//writeInstruction(lcd_Clear);
	//writeInstruction(lcd_LineOne | lcd_SetCursor);
	//writeString("[1] A vista");
	//writeInstruction(lcd_LineTwo | lcd_SetCursor);
	//writeString("[2] Parcelada");
	//_delay_ms(1000);
	//writeInstruction(lcd_Clear);
	//writeInstruction(lcd_LineOne | lcd_SetCursor);
	//writeString("[3] Estorno");
}
void tela_bloqueio_inicial(){
	writeString("     MicPay"); //ficar mais fiel ao pdf
	writeInstruction(lcd_SetCursor | lcd_LineTwo);
	writeString("       :)");
	_delay_ms(1000); 
	writeInstruction(lcd_Home | lcd_Clear);
}
void menu_pagamento(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Metodo de pgmt:");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("[1] deb [2] cred");
	
}
void menu_cartao(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Aprox./Insira");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Ou nro Cartao");
}
void tela_OK(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Op. realizada");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("com sucesso");	
	_delay_ms(4000);
}
void tela_CF(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Op. negada");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Conta com falha");
	_delay_ms(3000);
}
void tela_SI(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Op. negada");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Saldo insfc.");
	_delay_ms(3000);
}
void tela_SF(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Op. negada");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Senha com falha");
	_delay_ms(3000);
}
void tela_cartaoEstorno(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Cartao estorno:");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
}
void tela_valorEstorno(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Valor estorno:");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("R$ ");
}
void tela_estorno(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Senha (OP):");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
}
void tela_vendaAvista(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Valor (a vista):");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("R$ ");
}
void tela_vendaParcelada(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Valor (total):");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("R$ ");
}
void tela_nParcelas(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Nr. de parcelas:");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
}
void tela_senhaCartao(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Insira a senha: ");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
}
void tela_numCartao(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Cartao (numero): ");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
}
void tela_processandoVenda(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Aguardando ");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("servidor... ");
}
void menuADM(){
	_delay_ms(750);
	writeInstruction(lcd_Clear | lcd_Home);
	writeString("Selecione o que");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("quer configurar:");
	_delay_ms(1500);
	
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("1-Hora 2-Ver.Pen");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("3-Oper 4-Des.Pen");
}
void tela_instrucoes_configOper(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Selecione o oper");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("p/ hab/deshab:");
	_delay_ms(1500);	
}
void tela_continuar_modo_atual(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Deseja mudar de");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("modo? *:N  #:S");
}