#include <stdio.h>
#include <math.h>
#include "../Headers/vendas.h"
#include "../Headers/telas.h"
#include "../Headers/senha.h"
#include "../Headers/cartoes.h"
#include "../Headers/relogio.h"
#include "../Headers/modoAdm.h"
#include "../Headers/pendencias.h"


void processa_venda(int opcao){  // ativa fun��o requisitada segundo op��o selecionada no menu
	if(opcao == '1'){
		venda_avista();
	}else if(opcao == '2'){
		venda_parcelada();
	}else if(opcao == '3'){
		estorno();
	}
}

void formata_valor(char valor[]){	// formata o valor recebido para a forma [centena/dezena/unidade/dezena centavos/unidade centavos]
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

char metodo_pagamento(){			// fun��o recebe metodo de pagamento pelo teclado ([1] d�bito e [2] cr�dito)
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

char n_parcelas(){										// fun��o que recebe o n�mero de parcelas digitado no teclado
	char tecla = TECLA_INVALIDA,parcelas='1';
	short contador=0;
	
	tela_nParcelas();
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
	char senha_operador[] = {'0', '0', '0' ,'0'};	
	char cartao[] = {'0', '0', '0' ,'0', '0', '0'};
	char valor[] = {' ', ' ', ' ' ,' ',' '};
	short i,senhaValida=0;	
	char tipo_cartao; // parcelas = 0 = estorno
	

	char senhas[3][N_CARACSENHA] =  {	{'0','1','2','3'}, //adm
	{'1','2','3','4'}, //op 1
	{'2','3','4','5'}, //op 2
	};
	
	tela_estorno();
	recebe_senha(senha_operador);										// recebo a senha do operador
	
	for (i =0;i<3 ; i++){
		if(senhaValida == 0 ){
			senhaValida = validarSenha(senha_operador,senhas[i]);		// verifico a senha nas senhas cadastradas	
		}
	}
	
	if (senhaValida){													// realizo op de estorno se a senha for valida
		tela_valorEstorno();
		recebe_valor(valor);
		formata_valor(valor);
		tela_cartaoEstorno();
		tipo_cartao = aguarda_cartao();
		processa_estorno(tipo_cartao,valor,cartao); 
	}else{
		tela_SF();
	}
	
	_delay_ms(3000);
}

void venda_parcelada(){								// funcao para venda a vista considerando apenas 5 n�meros
	
	char senha[] = {'0', '0', '0' ,'0', '0', '0'};	
	char cartao[] = {'0', '0', '0' ,'0', '0', '0'};
	char valor[] = {' ', ' ', ' ' ,' ',' '};
	char metodo_pgmt,tipo_cartao,parcelas;
	char dataPags[12];
	short temp1, temp2, temp3,pagamento;
	tela_vendaParcelada();
	recebe_valor(valor);
	formata_valor(valor);	
	
	parcelas = n_parcelas();
	metodo_pgmt = metodo_pagamento();
	tipo_cartao = aguarda_cartao();
	
	pagamento = processa_pagamento(parcelas,tipo_cartao,metodo_pgmt,valor,senha,cartao);
	
	if(pagamento==1){
		//calculo pra mandar o valor das parcelas pra addPagamentoAgendado
		temp1 = (valor[4]-48)*100 + (valor[3]-48)*10 + (valor[2]-48); //parte antes da virgula para int
		parcelas = parcelas - 48; //parcelas de ascii paranumerico
		temp2 = temp1/parcelas; //pega as centenas dezenas e unidades da parcela
		temp3 = temp1%parcelas; // pega as dezenas e unidades de centavo da parcela
	
		temp1 = (valor[1]-48)*10 + (valor[0]-48); // pega os centavos
		temp1 = temp1/parcelas + temp3; //soma
	
		if(temp1 >= 100){ //verifica se centavos passou de 1 real
			temp1=temp1%100;
			temp2++;
		}
		//monta o vetor de valor
		valor[0]=temp2/100 + 48;
		valor[1]=(temp2/10)%10 + 48;
		valor[2]=(temp2%100)%10 + 48;
		valor[3]=temp1/10 + 48;
		valor[4]=temp1%10 + 48;
	
		sumDate(1); //data da primeira parcela
		for(temp1 = 0;temp1<6;temp1++)
			dataPags[temp1] = dataFUTURA[temp1];
	
		if(parcelas==3){ //se tiver mais parcelas
			sumDate(2); //data da segunda parcela
			for(temp1 = 0;temp1<6;temp1++)
			dataPags[temp1+6] = dataFUTURA[temp1];
			}else{
			dataPags[6] = 9; //flag de n�o ter parcela
		}
		//add pagamento agendado
		addPagamentoAgendado(cartao,valor,dataPags);
	}

}

void venda_avista(){								// funcao para venda a vista considerando apenas 4 n�meros
	char senha[] = {'0', '0', '0' ,'0', '0', '0'};	
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
	
	while(req != 1 && req != 3 && req != 5){

		if(tipo_cartao == 1){ // cart�o magnetico - vai cart�o (CM 123456 /r)
			if(req != 2){
				for(i=0;i<6;i++){
					cartao [i] = USART_recebe()	;		 // preenche cartao
				}
			}
		}
		
		if(tipo_cartao == 2){ // aproximacao - vai cart�o e senha (CW 123456 123456 /r) (n faz mt sentido no estorno mas deixei)
			for(i=0;i<6;i++){
				cartao [i] = USART_recebe(); // preenche cartao
			}

		}
		if(tipo_cartao == 3){ // digitar n� cart�o
				recebe_cartao(cartao);
		}
		req = requisicao_externa(cartao,cartao,valor,0);
		com_ext = 0;
	}
}

int processa_pagamento(int parcelas,int tipo_cartao,char metodo_pgmt, char valor[],char senha[],char cartao[]){
	short i, req=0, aux;
	char cartao_local=0;
	
	while(req != 1){						// enquanto a resposta de requisi��o n�o for igual a 1, fico no loop

		if(tipo_cartao == 1){ // cart�o magnetico - vai cart�o e pede senha (CM 123456 /r)
			if(req != 2){
				for(i=0;i<6;i++){
					cartao [i] = USART_recebe()	;		 // preenche cartao pelo valor recebido na serial
				}
			}
			recebe_senha(senha);						// preenche senha pelo valor digitado
		}
		if(tipo_cartao == 2){ // aproximacao - vai cart�o e senha (CW 123456 123456 /r)
			for(i=0;i<12;i++){
				if(i<6){cartao [i] = USART_recebe();}	// preenche cartao pelo valor recebido na serial
				else{senha [i-6] = USART_recebe();}		// preenche senha pelo valor recebido na serial
			}
		}
		if(tipo_cartao == 3){ // digitar n�cart�o e senha
			if(req != 2){
				recebe_cartao(cartao);					// preenche senha pelo cartao digitado
			}
			recebe_senha(senha);						// preenche senha pelo valor digitado
		}
		
		if(metodo_pgmt == 2){										// se o pagamento for no cart�o de cr�dito, verifica pra ver se for cart�o da FIRMA
			cartao_local = verificarContasLocais(cartao);
		}
			
		
		if(!cartao_local){											// se o cart�o n�o for do tipo local, faz requisi��o externa
			req = requisicao_externa(cartao,senha,valor,parcelas);
		}else{
			aux = executarVendaInterna(cartao_local,valor,senha);	// se for local, executa venda interna
			if(aux==1){
				req=1;
			}else if(aux==2){
				req=2;												// vai perguntar denovo a senha
			}else{
				req=1;
			}
			tela_vendaInterna(aux);
		}
		
		com_ext = 0;
		
		if(req==3 || req==4 || req==5){								// se retornou CF, SI ou falha de comunica��o ext, sai do loop
			return 0;
		}
	}
	return 1;
}

int requisicao_externa(char cartao[],char senha[], char valor[], int parcelas){
	char req[21];
	short i,tamanho = 0;
	
	
	if(parcelas == 1){									// para venda a vista (parcelas = 1)
		req[0] = 0x56;	// V em ascii
		req[1] = 0x56;	// V em ascii
		for(i=2;i<19;i++){								
			if(i<8){req[i] = cartao [i-2];}				// armazena cartao na requisicao
			if(i<14 && i>=8){req[i] = senha [i-8];}		// armazena senha na requisicao 
			if(i>13){ req[i] = valor[18-i];}			// armazena valor na requisicao
		}
		tamanho = 19;
	}else if(parcelas == 0){							// para estorno (parcelas = 0)
		req[0] = 0x45;	// E em ascii
		req[1] = 0x56;	// V em ascii
		for(i=2;i<13;i++){								
			if(i<8){req[i] = cartao [i-2];}				// armazena cartao na requisicao		
			else{ req[i] = valor[12-i];}				// armazena valor na requisicao	
		}
		tamanho = 13;
	}else{												// para venda  parcelada (parcelas > 1)
		req[0] = 0x56;	// V em ascii
		req[1] = 0x50;	// P em ascii
		for(i=2;i<20;i++){								
			if(i<8){req[i] = cartao [i-2];}				// armazena cartao na requisicao
			if(i<14 && i>=8){req[i] = senha [i-8];}		// armazena senha na requisicao
			if(i==14){ req[i] = parcelas;}				// armazena qtd de parcelas
			if(i>14){ req[i] = valor[19-i];}			// armazena valor na requisicao
		}
		tamanho = 20;
	}
	
	for(i=0;i<tamanho;i++){								//envia toda requisi��o para a serial
		USART_envia(req[i]);
	}
	USART_envia(0x0d);									// \r final
	
	com_ext = 1;										// abre flag de comunica��o externa
	
	while(1){
		if(parcelas==0){								// caso seja um estorno
			switch(USART_recebe() ){					// aguarda recebimento na serial
				case 0x4f: tela_OK();return 1;break;				// O(K) em ascii - OK -> volta pra tela inicial
				case 0x43: tela_CF();return 3;break;				// C(F) em ascii - CONTA COM FALHA -> volta pra tela inicial
				case 0: return 5; break;							// se for 0, erro de comunica��o ext
			}
		}else{
			switch(USART_recebe() ){					// aguarda recebimento na serial
				case 0x4f: tela_OK();return 1;break;				// O(K) em ascii - OK -> volta pra tela inicial
				case 0x43: tela_CF();return 3;break;				// C(F) em ascii - CONTA COM FALHA -> volta pra tela inicial
				case 0x53:											// S em ascii
					switch (USART_recebe()){
						case 0x46:										// F em ascii	- SENHA COM FALHA -> pergunta senha novamente
							tela_SF();
							return 2;
							break;
						case 0x49:										// I em ascii	- SALDO INSUFICIENTE -> volta pra tela inicial
							tela_SI();
							return 4;
							break;
					}
				case 0: return 5; break;				
			}
		}

	}
}

uint8_t transforma_int(char valor[]){	// transforma o valor do vetor de valores em int
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

void recebe_senha(char senha[]){ // fun��o recebe senha de 6 digitos  e armazena no vetor designado
	short contador=0;
	char tecla;
	tela_senhaCartao();

	while (1){
		tecla = teclaDebouce();
		if(tecla!= TECLA_INVALIDA){	// s� entra no loop se a tecla pressionada for valida ( uma das 12 do teclado )
			tecla = tecla+49;
			if(tecla == KEY_APAGAR_ASCII){ // caso tecla pressionada seja de apagar, deleta ultimo valor do char e apaga na tela
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

void recebe_cartao(char cartao[]){ // fun��o recebe cartao de 6 digitos  e armazena no vetor designado
	short contador=0;
	char tecla;
	tela_numCartao();

	while (1){
		tecla = teclaDebouce();
		if(tecla!= TECLA_INVALIDA){
			tecla = tecla+49;
			if(tecla == KEY_APAGAR_ASCII){		// caso tecla pressionada seja de apagar, deleta ultimo valor do char e apaga na tela
				if(contador>0){
					uint8_t aux = lcd_SetCursor | (uint8_t)(contador-1);
					writeInstruction(lcd_LineTwo | aux);
					writeString(" ");
					writeInstruction(lcd_LineTwo | aux);
					cartao[contador-1] = '0';
					contador --;
				}
				}else if(tecla == KEY_CONFIRMA_ASCII){ // caso tecla pressionada seja de confirma��o, sai do loop
					break;
				}else if(contador<6){					// caso nenhuma condi��o atendida, e n�o tiver 6 digitos armazenados ainda, armazena novo digito
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

void recebe_valor(char valor[]){	// fun��o recebe valor de 5 digitos (ou menos) e armazena no vetor designado
	char tecla = TECLA_INVALIDA;
	short contador = 0;
	short pos=4;
		
		while (1){
			tecla = teclaDebouce();
			if(tecla!= TECLA_INVALIDA){
				tecla = tecla+49;
				//USART_envia(tecla);
				if(tecla == KEY_APAGAR_ASCII){									// caso tecla pressionada seja de apagar, deleta ultimo valor do char e apaga na tela
					if(contador>0){
						uint8_t aux = lcd_SetCursor | (uint8_t)(contador+2);
						writeInstruction(lcd_LineTwo | aux);
						writeString(" ");
						writeInstruction(lcd_LineTwo | aux);
						valor[pos-contador+1] = ' ';
						contador --;
					}
					}else if(tecla == KEY_CONFIRMA_ASCII && valor[5]!=' '){		// caso tecla pressionada seja de confirma��o, sai do loop
						break;
					}else if(contador<5){										// caso nenhuma condi��o atendida, armazena digito
						if(tecla == KEY_0_ASCII){ // mudo o valor em ascii para zero
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