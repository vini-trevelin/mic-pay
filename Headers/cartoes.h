
#define LOCALCARDNUM 5
#define CARDSIZE 6
#define SENHASIZE 6
#define DIGITOS 5 // 999,99 reais
#define TRUE 1
#define FALSE 0

char verificarContasLocais(char cartaoLido[]); 						// verificada
char verificaCashBack (char indice,char valorVenda[]);
char addCartao(char cardNum[],char senha[], char saldoInicial[]); 	// verificada
void carregaSaldo(char saldoInic[]);								// verificada
void carregaNumCartao(char numCard[]); 								// verificada
void carregaSenha(char senha[]);  									// verificada
char executarVendaInterna(char indice,char valor[], char senha[]); 	
char subtraiSaldo(char indice,char venda[]);
char verificaSaldo (char indice, char valorVenda[],char temCashBack);
char verificaSenha(char indice, char senhaRecebida[]);
