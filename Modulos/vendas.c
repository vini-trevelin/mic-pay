#include <stdio.h>
#include <math.h>
#include "../Headers/vendas.h"

void processa_venda(int opcao){
		
	if(opcao == '1'){
		venda_avista();
	}
	else if(opcao == '2'){
		// venda_parcelada();
	}else if(opcao == '3'){
		// estorno();
	}
}
void formata_valor(char valor[]){	// provavelmente algo errado
	if(valor[2]=='0' && valor[1] == '0' && valor[0] == '0'){
		valor[2]=valor[3];
		valor[3]='0';
	}
}

char metodo_pagamento(){
	char tecla = TECLA_INVALIDA;
	menu_pagamento();
	while (1){
		tecla = teclaDebouce();
		if(tecla!= TECLA_INVALIDA){
			tecla = tecla+1;
			return tecla;	
		}
	}
}

int aguarda_cartao(){
	char tecla = TECLA_INVALIDA;
	// tipo_cartao = [1 -> cartao magnetico, 2 -> cartao wireless, 3 -> caso deseja digitar nº do cartão] 
	menu_cartao();
	while(1){
		tecla = teclaDebouce();
			if(USART_recebe() == 0x43){ // C em ascii, verifico se recebi algo pela serial ( SIMULAÇÃO DE CARTÃO NA MAQUININHA)
				switch(USART_recebe()){
					case 0x4D: return 1; //M em ascii
					case 0x57: return 2; // W em ascii 
				}
			}else if(tecla!= TECLA_INVALIDA){ // não pega por algum motivo
				return 3;
			}
	}
}

void venda_avista(){	// funcao para venda a vista considerando apenas 4 números
	char senha[] = {'0', '0', '0' ,'0', '0', '0'};	// to declarando aq pq n sei onde vou usar
	char cartao[] = {'0', '0', '0' ,'0', '0', '0'};
	
	char tecla = TECLA_INVALIDA,metodo_pgmt,tipo_cartao;
	short contador = 0;
	char valor[] = {'0', '0', '0' ,'0'};
	short pos=3;
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Valor (a vista):");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("R$ ");
	
	while (contador != 4){
		tecla = teclaDebouce();
		if(tecla!= TECLA_INVALIDA){
			tecla = tecla+49;
			//USART_envia(tecla);
			if(tecla == KEY_APAGAR_ASCII){
					if(contador>0){
						uint8_t aux = lcd_SetCursor | (uint8_t)(contador+2);
						writeInstruction(lcd_LineTwo | aux);
						writeString(" "); 
						writeInstruction(lcd_LineTwo | aux);
						valor[pos-contador+1] = '0';
						contador --;
					}
			}else if(tecla == KEY_CONFIRMA_ASCII){
				contador = 4;
				break;							
			}else{
				valor[pos-contador] = (tecla);
				uint8_t aux = lcd_SetCursor | (uint8_t)(contador+3);
				writeInstruction(lcd_LineTwo | aux);
				writeCharacter(tecla);
				writeInstruction(lcd_LineTwo | aux);
				contador++;
			}
		}
	}
	
	// print do valor 
	formata_valor(valor);
	USART_envia(valor[3]);
	USART_envia(valor[2]);
	USART_envia(valor[1]);
	USART_envia(valor[0]);
	//
	_delay_ms(2000);
	
	metodo_pgmt = metodo_pagamento();
	tipo_cartao = aguarda_cartao();
	//USART_envia(tipo_cartao+48);	//printa tipo do cartão
	processa_pagamento(1,tipo_cartao,metodo_pgmt,valor,senha,cartao); // se enviar 1 como parametro de parcelas == venda a vista
	
}
void processa_pagamento(int parcelas,int tipo_cartao,char metodo_pgmt, char valor[],char senha[],char cartao[]){
	short i;
	if(tipo_cartao == 1){ // cartão magnetico - vai cartão e pede senha
		
	}
	
	if(tipo_cartao == 2){ // aproximacao - vai cartão e senha (CW 123456 123456 /r)
		for(i=0;i<12;i++){
			if(i<6){cartao [i] = USART_recebe();} // preenche cartao
			else{senha [i-6] = USART_recebe();}	// preenche senha
		}
		requisicao_externa(cartao,senha,valor,parcelas);

	}
	
	if(tipo_cartao == 3){ // digitar nº e senha
			
	}
}
void requisicao_externa(char cartao[],char senha[], char valor[], int parcelas){		// por enquanto só funciona pra venda parcelada e a vista
	char req[20];
	short i, retorna=0;
	
	if(parcelas == 1){
		req[0] = 0x56;	// V em ascii
		req[1] = 0x56;	// V em ascii
			for(i=2;i<18;i++){								// tem q melhorar fiz a moda caralha
				if(i<8){req[i] = cartao [i-2];}				// armazena cartao na requisicao
				if(i<14 && i>=8){req[i] = senha [i-8];}		// armazena senha na requisicao esse and ai foi sacanagem
				if(i>13){ req[i] = valor[17-i];}			// armazena valor na requisicao
			}
			
			for(i=0;i<18;i++){								//envia pra serial
				USART_envia(req[i]);
			}
			
		USART_envia(0x0d);								// \r final
	}else{
		req[0] = 0x56;	// V em ascii
		req[1] = 0x50;	// P em ascii
			for(i=2;i<19;i++){								// tem q melhorar fiz a moda caralha
				if(i<8){req[i] = cartao [i-2];}				// armazena cartao na requisicao
				if(i<14 && i>=8){req[i] = senha [i-8];}		// armazena senha na requisicao esse and ai foi sacanagem
				if(i==14){ req[i] = parcelas;}				// armazena qtd de parcelas
				if(i>14){ req[i] = valor[i-15];}			// armazena valor na requisicao
			}
			
			for(i=0;i<19;i++){								//envia pra serial
				USART_envia(req[i]);
			}
			USART_envia(0x0d);								// \r final
	}
	
	while(1){
		
		switch(USART_recebe() ){
			case 0x4f: tela_OK();retorna = 0;break;				//O(K) em ascii
			case 0x43: tela_SF();retorna = 0;break;				//C(F) em ascii faz algo diferente
			case 0x53:											// S em ascii
				switch (USART_recebe()){
				case 0x46:										// F em ascii
					tela_SF();
					retorna = 0;
					break;
				case 0x49:					// I em ascii
					tela_SI();
					retorna = 0;
					break;
				}
		}
		
		if(retorna){break;}
	}
}
uint8_t transforma_int(char valor[]){
	short i, soma=0;
	if(valor[2]=='0'){
		valor[2]=valor[3];
		valor[3]=0;
	}
	for(i=3;i<0;i--){
		soma += valor[i]*(pow(10,i));
	}
	return soma;
}


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
	_delay_ms(3000);
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
