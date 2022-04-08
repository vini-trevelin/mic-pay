#include <avr/io.h>
#include "../Headers/lcd.h"
#include "../Headers/botoes.h"
#include "../Headers/relogio.h"
#include "../Headers/pendencias.h"
#include "../Headers/serial.h"
#include "../Headers/senha.h"
#include <util/delay.h>



/*
Matrizes de pagamentos AGENDADOS
cartoesA	-> Guarda o numero dos cartões
valParcelaA	-> Guarda o val da parcela
datasVenc	-> datas dos vencimentos (pegar de 6 em 6)(ex 131224130125 = 13/12/24 e 10/01/25 , 9????? se a data começar com 9 a parcela nõa existe)
																							   se começar com 8 já foi paga ou dada como pendente

Matrizes de pagamentos PENDENTES
cartoesP		-> Guarda o numero dos cartões com pendencia
valsPendentes	-> Guarda o val da parcela pendente

Trabalho de manter as linhas sincronizadas entre as matrizes
*/

//tenho q inicializar eles pra zero
static char cartoesA[NUMPAGSAGENDADOS][NUMDIGITOSCARTOES] = {{1,1,0,0,0,0},{2,2,0,0,0,0},{3,3,0,0,0,0}};//cartão1 = 147852, cartão2 = 127845
static char valParcelaA[NUMPAGSAGENDADOS][NUMDIGITOSVALORES] ={{0,1,1,0,0},{0,2,2,0,0},{0,3,3,0,0}}; //val parcela cartaõ1 = R$11 ; do cartaõ2 = R$22 cartão3 = R$33
static char datasVenc[NUMPAGSAGENDADOS][NUMDATAS] = {{0,1,0,2,2,2,9,0,0,0,0,0},{0,1,0,2,2,2,0,1,0,3,2,2},{0,1,0,2,2,2,0,1,0,3,2,2}}; // datasVen cartaõ1 = 01/02/22 ; datasVen cartaõ2 = 01/02/22 e 01/03/22

static char cartoesP[NUMPAGSAGENDADOS][NUMDIGITOSCARTOES] = {0};//{{7,8,9,4,5,5},{7,5,3,1,5,9},{1,5,9,7,5,3}}; //para testar
static char valParcelaP[NUMPAGSAGENDADOS][NUMDIGITOSVALORES] = {0}; //{{1,2,5,8,9},{1,4,7,5,8},{5,6,2,3}};

static short numPagsAgendados = 3; // pra saber quantos pagsAgendados tem
static short numPendecias = 0; //saber quantas pendencias temos

void addPagamentoAgendado(char cartao[], char valParcela[], char datasVenci[]){
	// se só tiver uma parcela, datasVenci tem q ter o 1° digito da data da segunda parcela com um 9(SEMPARCELA) (ex:2008259 = 20/08/25 e 9?????)
	short i;
	 
	//add cartão
	for(i=0;i<NUMDIGITOSCARTOES;i++)
		cartoesA[numPagsAgendados][i] = cartao[i]-48;
	
	//add val da parcela
	for(i=0;i<NUMDIGITOSVALORES;i++)
		valParcelaA[numPagsAgendados][i] = valParcela[i]-48;
		
	//add datas vencimento
	for(i=0;i<NUMDATAS;i++)
		datasVenc[numPagsAgendados][i] = datasVenci[i]-48;
		
	if(numPagsAgendados < NUMPAGSAGENDADOS) //tem q ser < pq uso como indice nas matrizes
		numPagsAgendados++;
		
// 	else  //Pode adicionar isso, mas teria q pensar em um jeito de resolver o problema, negar a compra dai?
// 		return NUMMAXDEPAGSAGENDADOS;
}

short removePagamentoAgendado(char cartao[]){
	//recebe o numero do cartão e vai remover a data da parcela mais proxima 
	//vai substituir o 1° numero da data mais proxima por oito 
	//ex:se datasVenc de um cartão é 130421130521 (13/04/21 e 13/05/21) vira 830421130521, assim em verificarPendencias sei q foi paga 
	//																		PARCELAPAGA = 8 
	//se só tiver uma parcela a ser removida, então ele vai zerar todas as infos desse cartão em cartoesA, valParcelas e dataVenc, dps ainda arruma a matriz
	
	short i, remover=-1;
	//acho o cartão que vai pagar uma parcela 
	for(i=0;i<numPagsAgendados;i++){
		if( comparaCartoes(cartoesA[i],cartao) == 1 ){ //se o cartão é igual
			remover = i; //sei qual indice pra remover a parcela
			break;
		}
	}
	
	if(remover == -1)
		return CARTAONAOENCONTRADO; //ver o q fazer com essa info
	
	/*
	Remoção de parcela:
	(data 1° parcela = p , data 2° parcela = s)
		Info das duas datas = ppppppssss s s -> segunda parcela começa no indice 6
							  01234567891011 */
	
	if(datasVenc[remover][0] != PARCELAPAGA_OUPENDENTE){ //se a primeira não foi paga(ou esta pendente) ainda, vai remover ela
		removePrimeiraParcela(remover);
	
	}else if(datasVenc[remover][6] != SEMPARCELA){ //se a segunda parcela existe (foi parcelado em 3x) remove ela então
													//não tem como a segunda parcela ter sido paga e ter o registro no sistema
		removeSegundaParcela(remover); 
	}
	
	return 1; //cartão encontrado e parcela paga 
}

short comparaCartoes(char cartao1[],char cartao2[]){
	short i;
	short igual=1;
	for (i=0;i<NUMDIGITOSCARTOES;i++){
		if(cartao1[i] != cartao2[i]){
			igual = 0;
			break;
		}	
	}
	return igual;
}

void removePrimeiraParcela(short remover){
	datasVenc[remover][0] = PARCELAPAGA_OUPENDENTE; //só precisa colocar o sinal q foi paga na primeira posição, ela que verifico em removePagamentoAgendado
	
	if(datasVenc[remover][6] == SEMPARCELA){ //mas se não vai ter outra parcela, pode tirar esse cartão das matrizes
		//no final percebi que era melhor colocar para zerar os valores em organizaPagamentos
		organizaPagamentos(remover);
	}
}

void removeSegundaParcela(short remover){
	//na verdade vai limpar as matrizes na linha da parcela removida e arrumar as matrizes
	//pq se vai remover a ultima possivel parcela, então não vai ter parcelas, obviamente vis a vis trivial
	organizaPagamentos(remover);
}

void organizaPagamentos(short remover){
	short i,j;
	char temp;
	
	//organiza cartoes
	for(i=remover;i<numPagsAgendados-1;i++){
		for(j=0;j<NUMDIGITOSCARTOES;j++){ 
			temp = cartoesA[i+1][j];
			cartoesA[i+1][j] = 0; //aqui que eu deleto a info da matriz
			cartoesA[i][j] = temp;
			}
		}
		
	//organiza valParcelas
	for(i=remover;i<numPagsAgendados-1;i++){
		for(j=0;j<NUMDIGITOSVALORES;j++){
			temp = valParcelaA[i+1][j];
			valParcelaA[i+1][j] = 0; //aqui que eu deleto a info da matriz
			valParcelaA[i][j] = temp;
		}
	}
	
	//organiza datas vencimento
	for(i=remover;i<numPagsAgendados-1;i++){
		for(j=0;j<NUMDATAS;j++){
			temp = datasVenc[i+1][j];
			datasVenc[i+1][j] = 0; //aqui que eu deleto a info da matriz
			datasVenc[i][j] = temp;
		}
	}
	
	numPagsAgendados--; //organiza pagamentos só é chamado quando se zera um pagamentoAgendado, então posso diminuir o numero de pags agendados
}

char existePendencia(){
	//vou olhar se tem algum valor em valParcelaP != 0 e retorna 1 se tiver
	if(numPendecias>0)
		return 1;
	else
		return 0;
}
char getNumPendecias(){
	return (char)numPendecias;
}

void verificarPendencia(){
	//012345678
	//dd/mm/aa
	char diaAtual[2],mesAtual[2],anoAtual[2];
	short i,pendAchada = -1;
	
	//pego as infos da data atual
	AtualizaStringDataHora();
	for(i=0;i<2;i++){
		diaAtual[i]=stringDATA[i]-48;
		mesAtual[i]=stringDATA[i+3]-48;
		anoAtual[i]=stringDATA[i+6]-48;
	}			
	            //012345 67891011
	//datasVenc = ddmmaa|ddmma a
	
	//for pra verificar se as primeiras parcelas tem pendencia
	for(i=0;i<numPagsAgendados;i++){
		if(datasVenc[i][0] != PARCELAPAGA_OUPENDENTE){
			if(anoAtual[0]>datasVenc[i][4] ||  ( anoAtual[0]==datasVenc[i][4] && anoAtual[1]>datasVenc[i][5] ) ){ //se o ano venceu
			
				pendAchada = i; //é uma pendecia
			
			}else if((anoAtual[0]==datasVenc[i][4] && anoAtual[1]==datasVenc[i][5]) && 
					(mesAtual[0]>datasVenc[i][2] || (mesAtual[0]==datasVenc[i][2] && mesAtual[1]>datasVenc[i][3])) ){ //se ano = mas mes venceu
						pendAchada = i;
					}
			
			else if(mesAtual[0]==datasVenc[i][2] && mesAtual[1]==datasVenc[i][3])
				if( diaAtual[0]>datasVenc[i][0] || ( diaAtual[0]==datasVenc[i][0] && diaAtual[1]>datasVenc[i][1]) ){ //se mes = o dia venceu
					pendAchada = i; 
			}
		
			if(pendAchada != -1)//add pendencia a matriz de pendencias
				addPendencias(pendAchada,1); //1 diz q foi na primeira data q achou 
		
			pendAchada = -1;
		}
	}
	pendAchada = -1;
	
	//para segunda parcela
	for(i=0;i<numPagsAgendados;i++){
		if(datasVenc[i][6] != PARCELAPAGA_OUPENDENTE && datasVenc[i][6] != SEMPARCELA){
			if(anoAtual[0]>datasVenc[i][10] ||  ( anoAtual[0]==datasVenc[i][10] && anoAtual[1]>datasVenc[i][11] ) ){ //se o ano venceu
				
				pendAchada = i; //é uma pendecia
				
			}else if((anoAtual[0]==datasVenc[i][10] && anoAtual[1]==datasVenc[i][11]) &&
			(mesAtual[0]>datasVenc[i][8] || (mesAtual[0]==datasVenc[i][8] && mesAtual[1]>datasVenc[i][9])) ){ //se ano = mas mes venceu
				pendAchada = i;
			}
			
			else if(mesAtual[0]==datasVenc[i][8] && mesAtual[1]==datasVenc[i][9])
					if( diaAtual[0]>datasVenc[i][6] || ( diaAtual[0]==datasVenc[i][6] && diaAtual[1]>datasVenc[i][7]) ){ //se mes = mas dia venceu
						pendAchada = i;
			}
			
			if(pendAchada != -1)//add pendencia a matriz de pendencias
				addPendencias(pendAchada,2); //2 diz q foi na segunda data q achou
			
			pendAchada = -1;
		}
	}
}

void addPendencias(short achada, char PouS){
	short i;
	for(i=0;i<NUMDIGITOSCARTOES;i++)
		cartoesP[numPendecias][i]=cartoesA[achada][i];
	
	for(i=0;i<NUMDIGITOSVALORES;i++)
		valParcelaP[numPendecias][i] = valParcelaA[achada][i];
		
	if(PouS == 1)
		removePrimeiraParcela(achada); 
	else
		removeSegundaParcela(achada);

	numPendecias++;
}

char removePendencias(char remover){
	if(remover+1 > numPendecias)
		return 0; //pediu pra remover uma pendencia q não existe
	//remover = remover-1; //correção pq indice começa em zero
	short i,j;
	char temp;
	
	//remove cartoesP
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	for(i=remover;i<numPendecias-1;i++){
		for(j=0;j<NUMDIGITOSCARTOES;j++){
			temp = cartoesP[i+1][j];	
			cartoesP[i+1][j] = 0; //aqui que eu deleto a info da matriz
			cartoesP[i][j] = temp;			
		}
	}
	//remove valParcelaP
	for(i=remover;i<numPendecias-1;i++){
		for(j=0;j<NUMDIGITOSVALORES-1;j++){
			temp = valParcelaP[i+1][j];
			valParcelaP[i+1][j] = 0; //aqui que eu deleto a info da matriz
			valParcelaP[i][j] = temp;
		}
	}
	numPendecias--;
	return 1; //removi a pendencia 
	
}

char printPendencias(char sobeDesce){
	if(numPendecias < 1)
		return 0; //sem pendencias pra printar
		
	if(sobeDesce == 1 && numPendecias >= 1){
		writeInstruction(lcd_Clear);
		writeInstruction(lcd_LineOne | lcd_SetCursor);
		printNumPendencia(0); //pend 1
		if(numPendecias > 1){
			writeInstruction(lcd_LineTwo | lcd_SetCursor);
			printNumPendencia(1); //pend 2
		}
		_delay_ms(3000);
	}else if(sobeDesce == 2 && numPendecias > 2){
		if(numPendecias > 2){
			writeInstruction(lcd_Clear);
			writeInstruction(lcd_LineOne | lcd_SetCursor);
			printNumPendencia(2);	//pend 3
		}
		if(numPendecias > 3){
			writeInstruction(lcd_LineTwo | lcd_SetCursor);
			printNumPendencia(3); //pend 4
		}
		_delay_ms(3000);
	}else if(sobeDesce == 3 && numPendecias >= 4){
		if(numPendecias > 4){
			writeInstruction(lcd_Clear);
			writeInstruction(lcd_LineOne | lcd_SetCursor);
			printNumPendencia(4); //pend 5
		}
		if(numPendecias > 5){
			writeInstruction(lcd_LineTwo | lcd_SetCursor);
			printNumPendencia(5); //pend 6
		}
		_delay_ms(3000);
	}
	return 1;
}

void printNumPendencia(short num){
	short i;
	//print num cartao
	for(i=0;i<NUMDIGITOSCARTOES;i++)
		writeCharacter(cartoesP[num][i]+48);
	writeString(" ");
	//print val parcela
	for(i=0;i<NUMDIGITOSVALORES;i++)
		writeCharacter(valParcelaP[num][i]+48);
	writeString(" -");
	writeCharacter(num+49);
}

void cobrarPagementosAgendados(){
	char diaAtual[2],mesAtual[2],anoAtual[2];
	short i, cobrar;
	char flagPagCobrado=0, pagCobrado=0;
	
	//pego as infos da data atual
	AtualizaStringDataHora();
	for(i=0;i<2;i++){
		diaAtual[i]=stringDATA[i]-48;
		mesAtual[i]=stringDATA[i+3]-48;
		anoAtual[i]=stringDATA[i+6]-48;
	}
	
	//012345 67891011 
	//ddmmaa|ddmma a
	//comparar com primeira coluna de datasVenc 
	for(i=0,cobrar=-1;i<numPagsAgendados;i++){
		if(datasVenc[i][0] != PARCELAPAGA_OUPENDENTE){ //verifica a primeira data
			if(datasVenc[i][0]==diaAtual[0] && datasVenc[i][1]==diaAtual[1] && 
			datasVenc[i][2]==mesAtual[0] && datasVenc[i][3]==mesAtual[1] &&
			datasVenc[i][4]==anoAtual[0] && datasVenc[i][5]==anoAtual[1]){ //se é o dia de cobrar
				cobrar = i;
			}
		}
		if(datasVenc[i][6] != SEMPARCELA){ //verifica a segunda
			if(datasVenc[i][6]==diaAtual[0] && datasVenc[i][7]==diaAtual[1] &&
			datasVenc[i][8]==mesAtual[0] && datasVenc[i][9]==mesAtual[1] &&
			datasVenc[i][10]==anoAtual[0] && datasVenc[i][11]==anoAtual[1]){
				cobrar = i;	
			}
		}
		
		if(cobrar != -1){
			pagCobrado = enviarPedidoDePagamento(cobrar);
			cobrar = -1;
			flagPagCobrado = 1;
			if(pagCobrado) //caso o pag tenha sido cobrado é preciso voltar na matriz pq organizaPagamentos()
				i--;			//vai fazer todos subirem uma posição
		}
	}
	
	if(flagPagCobrado == 1){ //se não tiver sido cobrado pag não tem pq mudar a tela e interromper a maquina
		tela_fimDacobranca();
		tela_precioneOK();
	}
}
	
char enviarPedidoDePagamento(short cobrar){
	char cartao[NUMDIGITOSCARTOES];
	char pedido[13], resultado;
	short i;
	
	for(i=0;i<NUMDIGITOSCARTOES;i++){ //pega num do cartao e prepara o pedido
		cartao[i] = cartoesA[cobrar][i];
		pedido[i+2] = cartao[i]+48;
	}
	
	for(i=0;i<NUMDIGITOSVALORES;i++){ //prepara o pedido
		pedido[i+8] = valParcelaA[cobrar][i]+48;
	}
	//AP cccccc vvvvv \r
	pedido[0] = 'A'; // finaliza o pedido
	pedido[1] = 'P';
	
	resultado = enviarPedidoSerial(pedido);
	
	if(resultado == 1){
		removePagamentoAgendado(cartao);
		tela_operacaoConcluida();
		return 1;
	}else if(resultado == 2){
		tela_pagNencontrado();
	}
	return 0;
}

char enviarPedidoSerial(char pedido[]){
	short i;
	char recebe_serial[3];
	tela_cobrarPagAgendado();
	tela_AguardandoPagAgendado();
	
	for(i=0;i<13;i++){
		USART_envia(pedido[i]);
	}
	USART_envia(0x0d);
	
	recebe_serial[0] = USART_recebe();
	recebe_serial[1] = USART_recebe();
	recebe_serial[2] = USART_recebe();
	writeInstruction(lcd_Clear);
	
	if(recebe_serial[0] == 0x4f){
		return 1;
	}else if(recebe_serial[0] == 0x50){
		return 2;
	}
	return 0;
}
