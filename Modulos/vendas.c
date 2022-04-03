#include <stdio.h>
#include <math.h>
#include "../Headers/vendas.h"
#include "../Headers/telas.h"
#include "../Headers/senha.h"
#include "../Headers/cartoes.h"



void processa_venda(int opcao){
	if(opcao == '1'){
		venda_avista();
	}
	else if(opcao == '2'){
		venda_parcelada();
	}else if(opcao == '3'){
		estorno();
	}
}

void formata_valor(char valor[]){	// provavelmente algo errado gambiarra fudida
	short i;
	
	if(valor[4] != '0' && valor[3]==' ' && valor[2]==' ' && valor[1] == ' ' && valor[0] == ' '){
		valor[2]=valor[4];
		valor[4]=' ';
	}
	if(valor[4]!=' ' && valor[3]!=' ' && valor[2]==' ' && valor[1] == ' ' && valor[0] == ' '){
		valor[2] = valor[3];
		valor[3]= valor[4];
		valor[4]=' ';
	}
	
	for(i=0;i<5;i++){
		if(valor[i]==' ')
			valor[i] = '0';
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
	// tipo_cartao = [1 -> cartao magnetico, 2 -> cartao wireless, 3 -> caso deseja digitar n� do cart�o] 
	menu_cartao();
	while(tecla == TECLA_INVALIDA){
		tecla = teclaDebouce();
		//USART_envia(tecla+49);
		if((UCSR0A & (1<<RXC0))){		// preciso desse if pra n ficar preso no loop
			if(USART_recebe() == 0x43){ // C em ascii, verifico se recebi algo pela serial ( SIMULA��O DE CART�O NA MAQUININHA)
				switch(USART_recebe()){
					case 0x4D: return 1; //M em ascii
					case 0x57: return 2; // W em ascii 
					default:break;
				}
			}
		}
	}
	return 3;
}

char n_parcelas(){
	char tecla = TECLA_INVALIDA,parcelas='1';
	short contador=0;
	
	tela_nParcelas();
	while (1){
		tecla = teclaDebouce();
		if(tecla!= TECLA_INVALIDA){
			tecla = tecla+49;
			//USART_envia(tecla);
			if(tecla == KEY_APAGAR_ASCII){
				if(contador>0){
					uint8_t aux = lcd_SetCursor | (uint8_t)(contador-1);
					writeInstruction(lcd_LineTwo | aux);
					writeString(" ");
					writeInstruction(lcd_LineTwo | aux);
					parcelas = '1';
					contador --;
				}
			}else if(tecla == KEY_CONFIRMA_ASCII && parcelas!='1'){
				break;
			}else if(contador<1 && tecla < 52  && tecla > 49){
				if(tecla == KEY_0_ASCII){
					tecla = 48;
				}
				parcelas = (tecla);
				uint8_t aux = lcd_SetCursor | (uint8_t)(contador);
				writeInstruction(lcd_LineTwo | aux);
				writeCharacter(tecla);
				writeInstruction(lcd_LineTwo | aux);
				contador++;
			}
		}
	}
	return parcelas;
}

void estorno(){
	char senha_operador[] = {'0', '0', '0' ,'0', '0', '0'};	// to declarando aq pq n sei onde vou usar
	char cartao[] = {'0', '0', '0' ,'0', '0', '0'};
	char valor[] = {' ', ' ', ' ' ,' ',' '};
	short i,senhaValida=0;	
	char tipo_cartao; // parcelas = 0 = estorno
	
	// declarei denovo pq n sei se vai ficar global ou oq ---- MUDAR

	char senhas[3][N_CARACSENHA] =  {	{0,1,2,3}, //adm
	{1,2,3,4}, //op 1
	{2,3,4,5}, //op 2
	};

	/// ----------------------------------------------------------
	
	tela_estorno();
	recebe_senha(senha_operador);
	
	for (i =0;i<3 ; i++){
		if(senhaValida == 0 ){
			senhaValida = validarSenha(senha_operador,senhas[i]);
		}
	}
	if (senhaValida){
		tela_valorEstorno();
		recebe_valor(valor);
		formata_valor(valor);
		tela_cartaoEstorno();
		tipo_cartao = aguarda_cartao();
		processa_estorno(tipo_cartao,valor,cartao); 
	}else{
		tela_SF();
	}
	
	_delay_ms(5000);
}

void venda_parcelada(){	// funcao para venda a vista considerando apenas 5 n�meros
	
	char senha[] = {'0', '0', '0' ,'0', '0', '0'};	// to declarando aq pq n sei onde vou usar
	char cartao[] = {'0', '0', '0' ,'0', '0', '0'};
	char valor[] = {' ', ' ', ' ' ,' ',' '};
	char metodo_pgmt,tipo_cartao,parcelas;

	tela_vendaParcelada();
	recebe_valor(valor);
	formata_valor(valor);	
		
	// print do valor
	//USART_envia(valor[4]);
	//USART_envia(valor[3]);
	//USART_envia(valor[2]);
	//USART_envia(valor[1]);
	//USART_envia(valor[0]);
	//
	
	parcelas = n_parcelas();
	metodo_pgmt = metodo_pagamento();
	tipo_cartao = aguarda_cartao();
	
	processa_pagamento(parcelas,tipo_cartao,metodo_pgmt,valor,senha,cartao); 
	
}

void venda_avista(){	// funcao para venda a vista considerando apenas 4 n�meros
	char senha[] = {'0', '0', '0' ,'0', '0', '0'};	// to declarando aq pq n sei onde vou usar
	char cartao[] = {'0', '0', '0' ,'0', '0', '0'};
	
	char metodo_pgmt,tipo_cartao;
	char valor[] = {' ', ' ', ' ' ,' ',' '};
	
	tela_vendaAvista();
	recebe_valor(valor);
	formata_valor(valor);
	
	metodo_pgmt = metodo_pagamento();
	tipo_cartao = aguarda_cartao();
	processa_pagamento(1,tipo_cartao,metodo_pgmt,valor,senha,cartao); // se enviar 1 como parametro de parcelas == venda a vista
	
}

void processa_estorno(int tipo_cartao, char valor[],char cartao[]){
	short i, req=0;
	
	while(req != 1){

		if(tipo_cartao == 1){ // cart�o magnetico - vai cart�o e pede senha (CM 123456 /r)
			if(req != 2){
				for(i=0;i<6;i++){
					cartao [i] = USART_recebe()	;		 // preenche cartao
				}
			}
		}
		
		if(tipo_cartao == 2){ // aproximacao - vai cart�o e senha (CW 123456 123456 /r)
			for(i=0;i<6;i++){
				cartao [i] = USART_recebe(); // preenche cartao
			}

		}
		if(tipo_cartao == 3){ // digitar n� e senha
				recebe_cartao(cartao);
		}
		req = requisicao_externa(cartao,cartao,valor,0);
	}
}

void processa_pagamento(int parcelas,int tipo_cartao,char metodo_pgmt, char valor[],char senha[],char cartao[]){
	short i, req=0, aux;
	char cartao_local=0;
	
	while(req != 1){

		if(tipo_cartao == 1){ // cart�o magnetico - vai cart�o e pede senha (CM 123456 /r)
			if(req != 2){
				for(i=0;i<6;i++){
					cartao [i] = USART_recebe()	;		 // preenche cartao
				}
			}
			recebe_senha(senha);
		}
	
		if(tipo_cartao == 2){ // aproximacao - vai cart�o e senha (CW 123456 123456 /r)
			for(i=0;i<12;i++){
				if(i<6){cartao [i] = USART_recebe();} // preenche cartao
				else{senha [i-6] = USART_recebe();}	// preenche senha
			}

		}
		if(tipo_cartao == 3){ // digitar n� e senha
			
			if(req != 2){
				recebe_cartao(cartao);
				
			}
			recebe_senha(senha);
			
		}
		
		if(metodo_pgmt == 2){										// se o pagamento for no cart�o de cr�dito, verifica pra ver se for cart�o da FIRMA
			cartao_local = verificarContasLocais(cartao);
		}
			
		
		if(!cartao_local){											// se o cart�o n�o for do tipo local, faz requisi��o externa
			req = requisicao_externa(cartao,senha,valor,parcelas);
		}else{
			aux = executarVendaInterna(cartao_local,valor,senha);	
			if(aux==1){
				req=1;
			}else if(aux==2){
				req=2;												// vai perguntar denovo a senha
			}else{
				req=1;
			}
			tela_vendaInterna(aux);
			//mostraSaldoNaSerial(cartao_local);
			
		}
			
	}
}

int requisicao_externa(char cartao[],char senha[], char valor[], int parcelas){
	char req[21];
	short i,tamanho = 0;
	
	if(parcelas == 1){										// para venda a vista	------------------------------------------------------
		req[0] = 0x56;	// V em ascii
		req[1] = 0x56;	// V em ascii
		for(i=2;i<19;i++){								// tem q melhorar fiz a moda caralha
			if(i<8){req[i] = cartao [i-2];}				// armazena cartao na requisicao
			if(i<14 && i>=8){req[i] = senha [i-8];}		// armazena senha na requisicao esse and ai foi sacanagem
			if(i>13){ req[i] = valor[18-i];}			// armazena valor na requisicao
		}
		tamanho = 19;
	}else if(parcelas == 0){							// para estorno	-------------------------------------------------------------
		req[0] = 0x45;	// V em ascii
		req[1] = 0x56;	// P em ascii
		for(i=2;i<13;i++){								
			if(i<8){req[i] = cartao [i-2];}				
			else{ req[i] = valor[12-i];}			
		}
		tamanho = 13;
	}else{												// para venda  parcelada	---------------------------------------------------	
		req[0] = 0x56;	// V em ascii
		req[1] = 0x50;	// P em ascii
		for(i=2;i<20;i++){								// tem q melhorar fiz a moda caralha
			if(i<8){req[i] = cartao [i-2];}				// armazena cartao na requisicao
			if(i<14 && i>=8){req[i] = senha [i-8];}		// armazena senha na requisicao esse and ai foi sacanagem
			if(i==14){ req[i] = parcelas;}				// armazena qtd de parcelas
			if(i>14){ req[i] = valor[19-i];}			// armazena valor na requisicao
		}
		tamanho = 20;
	}
	
	for(i=0;i<tamanho;i++){								//envia pra serial
		USART_envia(req[i]);
	}
	USART_envia(0x0d);								// \r final

	tela_processandoVenda();
	
	while(1){
		
		switch(USART_recebe() ){
			case 0x4f: tela_OK();return 1;break;				// O(K) em ascii - OK -> volta pra tela inicial
			case 0x43: tela_CF();return 1;break;				// C(F) em ascii - CONTA COM FALHA -> volta pra tela inicial
			case 0x53:											// S em ascii
			switch (USART_recebe()){
				case 0x46:										// F em ascii	- SENHA COM FALHA -> pergunta senha novamente
				tela_SF();
				return 2;
				break;
				case 0x49:										// I em ascii	- SALDO INSUFICIENTE -> volta pra tela inicial
				tela_SI();
				return 1;
				break;
			}
		}
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

void recebe_senha(char senha[]){
	short contador=0;
	char tecla;
	tela_senhaCartao();

	while (1){
		tecla = teclaDebouce();
		if(tecla!= TECLA_INVALIDA){
			tecla = tecla+49;
			if(tecla == KEY_APAGAR_ASCII){
				if(contador>0){
					uint8_t aux = lcd_SetCursor | (uint8_t)(contador-1);
					writeInstruction(lcd_LineTwo | aux);
					writeString(" ");
					writeInstruction(lcd_LineTwo | aux);
					senha[contador-1] = '0';
					contador --;
				}
			}else if(tecla == KEY_CONFIRMA_ASCII){
				break;
			}else if(contador<6){
				if(tecla == KEY_0_ASCII){
					tecla = 48;
				}
				senha[contador] = (tecla);
				uint8_t aux = lcd_SetCursor | (uint8_t)(contador);
				writeInstruction(lcd_LineTwo | aux);
				writeString("*");
				writeInstruction(lcd_LineTwo | aux);
				contador++;
			}
		}
	}
}

void recebe_cartao(char cartao[]){
	short contador=0;
	char tecla;
	tela_numCartao();

	while (1){
		tecla = teclaDebouce();
		if(tecla!= TECLA_INVALIDA){
			tecla = tecla+49;
			if(tecla == KEY_APAGAR_ASCII){
				if(contador>0){
					uint8_t aux = lcd_SetCursor | (uint8_t)(contador-1);
					writeInstruction(lcd_LineTwo | aux);
					writeString(" ");
					writeInstruction(lcd_LineTwo | aux);
					cartao[contador-1] = '0';
					contador --;
				}
				}else if(tecla == KEY_CONFIRMA_ASCII){
					break;
				}else if(contador<6){
					if(tecla == KEY_0_ASCII){
						tecla = 48;
					}
					cartao[contador] = (tecla);
					uint8_t aux = lcd_SetCursor | (uint8_t)(contador);
					writeInstruction(lcd_LineTwo | aux);
					writeCharacter(tecla);
					writeInstruction(lcd_LineTwo | aux);
					contador++;
			}
		}
	}
}

void recebe_valor(char valor[]){	// f recebe valor de 5 digitos (ou menos)
	char tecla = TECLA_INVALIDA;
	short contador = 0;
	short pos=4;
		
		while (1){
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
						valor[pos-contador+1] = ' ';
						contador --;
					}
					}else if(tecla == KEY_CONFIRMA_ASCII && valor[5]!='0'){
						break;
					}else if(contador<5){
						if(tecla == KEY_0_ASCII){
							tecla = 48;
						}
					valor[pos-contador] = (tecla);
					uint8_t aux = lcd_SetCursor | (uint8_t)(contador+3);
					writeInstruction(lcd_LineTwo | aux);
					writeCharacter(tecla);
					writeInstruction(lcd_LineTwo | aux);
					contador++;
				}
			}
		}
}