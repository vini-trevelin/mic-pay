
#define LOCALCARDNUM 5
#define CARDSIZE 6
#define SENHASIZE 6
#define DIGITOS 5 // 999,99 reais
#define TRUE 1
#define FALSE 0

char verificarContasLocais(char cartaoLido[]); 						// verificada
char verificaCashBack (char indice,char valorVenda[]);				// verificada
char addCartao(char cardNum[],char senha[], char saldoInicial[]); 	// REVISAR FORMA DE ENTRADA DOS DADOS
void carregaSaldo(char saldoInic[]);								// verificada
void carregaNumCartao(char numCard[]); 								// verificada
void carregaSenha(char senha[]);  									// verificada
char executarVendaInterna(char indice,char valor[], char senha[]);  // verificada	
void subtraiSaldo(char indice,char venda[]);					    // verificada
char verificaSaldo (char indice, char valorVenda[]);				// verificada
char verificaSenha(char indice, char senhaRecebida[]);				// verificada
void atualizaSenha(char indice,char novaSenha[]); 					// REVISAR FORMA DE ENTRADA DOS DADOS
char adicionaSaldo(char indice,char valorAdd[]);					// REVISAR FORMA DE ENTRADA DOS DADOS

void mostraSaldoNaSerial(char indice)	;							// SÓ PRA TESTE