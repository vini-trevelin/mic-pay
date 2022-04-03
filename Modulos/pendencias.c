#include <avr/io.h>
#include "../Headers/pendencias.h"
#include "../Headers/lcd.h"
#include "../Headers/botores.h"

#define NUMPAGSAGENDADOS 6 //NUM de pag que podem estar agendados
#define NUMDIGITOSCARTOES 6 //num de digitos em um cartão
#define NUMDIGITOSVALORES 5 //num de digitos em val de parcelas
#define NUMDATAS 12 //2*6 (max duas datas, cada uma com 6 digitos cada)

#define CARTAONAOENCONTRADO 0 
#define PARCELAPAGA_OUPENDENTE 8
#define SEMPARCELA 9


/*
Matrizes de pagamentos AGENDADOS
cartoesA	-> Guarda o numero dos cartões
valParcelaA	-> Guarda o val da parcela
datasVenc	-> datas dos vencimentos (pegar de 6 em 6)(ex 131224130125 = 13/12/24 e 10/01/25 , 9????? se a data começar com 9 a parcela nõa existe)

Matrizes de pagamentos PENDENTES
cartoesP		-> Guarda o numero dos cartões
valsPendentes	-> Guarda o val da parcela

Trabalho de manter as linhas sincronizadas entre as matrizes
*/

//tenho q inicializar eles pra zero
static char cartoesA[NUMPAGSAGENDADOS][NUMDIGITOSCARTOES] = {0};
static char valParcelaA[NUMPAGSAGENDADOS][NUMDIGITOSVALORES] = {0};
static char datasVenc[NUMPAGSAGENDADOS][NUMDATAS] = {0};

static char cartoesP[NUMPAGSAGENDADOS][NUMDIGITOSCARTOES] = {0}; //talvez mais linhas, já que cada cartão poderia ter 2 pendencias teoricamente
static char valParcelaP[NUMPAGSAGENDADOS][NUMDIGITOSVALORES] = {0};

static char numPagsAgendados = 0; // pra saber quantos pagsAgendados tem
static char numPendecias = 0; //saber quantas pendencias temos

//as funcs espera receber os valores em ascii, mas vão armazenar em numerico
void addPagamentoAgendado(char cartao[], char valParcela[], char datasVenci[]); //para add no caso de compra com cartao
short removePagamentoAgendado(char cartao[]); //para remover quando o pagamento for recebido (12,18 ou 22)(sempre remove uma parcela, verifica qual deve ser)
void removePrimeiraParcela(short remover);
void removeSegundaParcela(short remover);
void organizaPagamentos(); //para organizar dps q um foi removido

char existePendecia(); //olha se tem alguma pendecia, retorna se tem (as 12, 18 e 22) (func pra deixar led ligada na pratica) 
void verificarPendencia(char dia[],char mes[], char ano[]); //add um pag agendado que expirou em pendencias, rodar as 22h sempre
void removePendencias(char pendencia); //pro adm remover pendencias manualmente (vai receber o indice dela)(talvez retorne algo dizendo se o selecionado é valido)
void organizaPendencias(); //pra organizar dps q uma for removida

void comparaCartoes(char cartao1[],char cartao2[]);

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
		datasVenc[numPagsAgendados][i] = datasVenci[i];
		
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
		if( comparaCartoes(cartoesA[i],cartao) ){ //se o cartão é igual
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
	short i;
	datasVenc[remover][0] = PARCELAPAGA_OUPENDENTE; //só precisa colocar o sinal q foi paga na primeira posição, ela que verifico em removePagamentoAgendado
	
	if(datasVenc[remover][6] == SEMPARCELA){ //mas se não vai ter outra parcela, pode tirar esse cartão das matrizes
		//tira infos das matrizes
// 		
// 		//tira cartão
// 		for(i=0;i<NUMDIGITOSCARTOES;i++)
// 			cartoesA[remover][i] = 0;
// 		
// 		//tira val da parcela
// 		for(i=0;i<NUMDIGITOSVALORES;i++)
// 			valParcelaA[remover][i] = 0;
// 		
// 		//tira datas vencimento
// 		for(i=0;i<NUMPAGSAGENDADOS;i++)
// 			datasVenc[remover][i] = 0;
		
		//no final percebi que era melhor colocar os zerar os valores em organizaPagamentos
		organizaPagamentos(remover);
	}
}

void removeSegundaParcela(short remover){
	//na verdade vai limpar as matrizes na linha da parcela removida e arrumar as matrizes
	//pq se vai remover a ultima possivel parcela, então não vai ter parcelas, obviamente vis a vis trivial
// 	
// 	//tira infos das matrizes
// 	//tira cartão
// 	short i;
// 	for(i=0;i<NUMDIGITOSCARTOES;i++)
// 		cartoesA[remover][i] = 0;
// 	
// 	//tira val da parcela
// 	for(i=0;i<NUMDIGITOSVALORES;i++)
// 		valParcelaA[remover][i] = 0;
// 	
// 	//tira datas vencimento
// 	for(i=0;i<NUMPAGSAGENDADOS;i++)
// 		datasVenc[remover][i] = 0;

	//no final percebi que era melhor colocar os zerar os valores em organizaPagamentos
	organizaPagamentos(remover);
}

void organizaPagamentos(short remover){
	short i,j;
	char temp;
	
	//organiza cartoes
	for(i=remover;i<numPagsAgendados;i++){
		for(j=0;i<NUMDIGITOSCARTOES;j++){ 
			temp = cartoesA[i+1][j];
			cartoesA[i+1][j] = 0; //aqui que eu deleto a info da matriz
			cartoesA[i][j] = temp;
			}
		}
		
	//organiza valParcelas
	for(i=remover;i<numPagsAgendados;i++){
		for(j=0;i<NUMDIGITOSVALORES;j++){
			temp = valParcelaA[i+1][j];
			valParcelaA[i+1][j] = 0; //aqui que eu deleto a info da matriz
			valParcelaA[i][j] = temp;
		}
	}
	
	//organiza datas vencimento
	for(i=remover;i<numPagsAgendados;i++){
		for(j=0;i<NUMDATAS;j++){
			temp = datasVenc[i+1][j];
			datasVenc[i+1][j] = 0; //aqui que eu deleto a info da matriz
			datasVenc[i][j] = temp;
		}
	}
	
	numPagsAgendados--; //organiza pagamentos só é chamado quando se zera um pagamentoAgendado, então posso diminuir o numero de pags agendados
}

char existePendecia(){
	//vou olhar se tem algum valor em valParcelaP != 0 e retorna 1 se tiver
	if(numPendecias>0)
		return 1;
	else
		return 0;
	
}

void verificarPendencia(char dia[],char mes[], char ano[]){
	//olhar as datas de vencimento de datasVenc e add a pendencia 
	
	/*
	(tem q comparar para todos os pagamentosAgendados
	ou seja for(i=0;i<numPagsAgendados;i++)
				compara 
				add se precisar )
				
	comparação:(tem q comparar a segunda parcela tb)(se o 1° digito do dia é 8 quer dizer q já foi pago ou dado como pendente, não deve acusar)
				(o 8 no 1° digito do dia garante que podemos esperar q o cara pague a segunda parcela, mesmo tendo a 1° pendente
				edge case fudido mas ta ai suhsuhsushushushushushs)
				
	(agendado é o que esta em datasVenc, Agora é o do sistema)
	se (anoAgora >= anoAgendado || mesAgora > mesAgendado) -> pendencia
	ou se mesAgora == mesAgendado && diaAgora > diaAgendado -> pendencia
	
	Adição:
	pega o indice da linha que foi achado a pendencia e faz:
	(digamos que o indice seja "pendencia")
	
	
	//copia o num do cartão
	for(i=0;i<NUMDIGITOSCARTOES;i++)
		cartoesP[numPendecias][i] = cartoesA[pendencia][i];
	
	//copia o val da pendencia
	for(i=0;i<NUMDIGITOSVALORES;i++)
		valParcelaP[numPendecias][i] = valParcelaA[pendencia][i];
		
	//tb deve add o famoso 8 na data que a pendencia foi criada
	datasVenc[pendencia][0 OU 6, tem que ver] = PARCELAPAGA_OUPENDENTE;
	
	numPendecias++; //mais uma pendecia (numPendencias tem um limite) */
}