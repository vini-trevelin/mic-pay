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
	_delay_ms(750); 
	writeInstruction(lcd_SetCursor | lcd_LineTwo);
	writeString("       ;D");
	_delay_ms(750);
	writeInstruction(lcd_SetCursor | lcd_LineTwo);
	writeString("       :)");
	_delay_ms(750);
	writeInstruction(lcd_Clear);
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
void tela_vendaInterna(int aux){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	if(aux==1){
		writeString("Venda interna");
		writeInstruction(lcd_LineTwo | lcd_SetCursor);
		writeString("realizada ");
		_delay_ms(2000);
	}else if(aux==0){
		writeString("Venda cancelada");
		writeInstruction(lcd_LineTwo | lcd_SetCursor);
		writeString("Limite atingido ");
		_delay_ms(2000);
	}else{
		tela_SF();
	}
	
}

void tela_reaproxime(int aux){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Erro de Leitura");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Tente Novamente");
	_delay_ms(2000);
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
void tela_processandoVenda2(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Aguardando ");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("servidor... (2)");
}
void tela_processandoVenda3(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Aguardando ");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("servidor... (3)");
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
	writeString("1-Data 2-Ver.Pen");
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
void tela_instrucoes_configDiaMesAno(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("formato: ");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("    dd/mm/yy");
	_delay_ms(1500);
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("insira a nova");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("data: ");
}
void tela_instrucoes_configHoraMinSeg(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("formato: ");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("    hh:mm:ss");
	_delay_ms(1500);
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("insira a nova");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("hora: ");
}
void tela_operacaoConcluida(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Operacao");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Concluida!");
	_delay_ms(1000);
}
void tela_dataInvalida(){
	
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Data INVALIDA!");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Data inalterada");
	_delay_ms(1000);

}
void tela_operadorDesabilitado(char oper){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Operador ");
	writeCharacter(oper+48);
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Desabilitado");
	_delay_ms(1000);
}
void tela_semPendencias(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Nao ha ");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Pendencias");
	_delay_ms(1000);
}
void tela_opcaoInvalida(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("OPCAO ");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("INVALIDA");
	_delay_ms(1000);
}
void tela_cobrarPagAgendado(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Cobrando");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Pags Agendados");
	_delay_ms(1000);
}
void tela_AguardandoPagAgendado(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Aguardando");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Sistema");
	_delay_ms(1000);
}
void tela_instrucoes_remPendencia(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Informe o numero");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("da pendencia:");
	_delay_ms(1000);
}
void tela_infos_printPend(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Ordem das infos:");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("cartao val.Pen");
	_delay_ms(2000);
}
void tela_dataAtual(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne |lcd_SetCursor);
	writeString("Data: ");
	writeInstruction(lcd_LineOne | (lcd_SetCursor|(uint8_t)5));
	AtualizaStringDataHora();
	writeString(stringDATA);
	writeString(" E#");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Hora: ");
	writeInstruction(lcd_LineTwo | (lcd_SetCursor+(uint8_t)5));
	writeString(stringHORA);
	writeString(" V*");
}
void tela_erroCom(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Erro na");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Comunicacao Ext.");
	_delay_ms(2000);
}
void tela_precioneOK(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("CONFIRMA ");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Para continuar");
	_delay_ms(2000);
}
void tela_fimDacobranca(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Cobrancas");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("finalizadas");
	_delay_ms(2000);
}
void tela_pagNencontrado(){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("pagamento nao");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("localizado");
	_delay_ms(2000);
}
