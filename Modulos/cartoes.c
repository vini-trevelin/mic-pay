
//==================================================================
/*
 *  Funções que devem ser acessadas externamente
 *  char verificaContasLocais (char cartaoLido[])  retorna a (posicaoDoCartao+ 1) e zero se não Existir
 *  char addCartao(char cardNum[],char senha[], char saldoInicial[]) # cria uma nova conta se possivel (true/1) ou false(0);
 *  char executarVendaInterna(char indice,char valor[], char senha[]) # executa uma venda se possivel (true/1)  ou retorna erro (false/0)
 *  void atualizaSenha(char indice,char novaSenha[])  #atualiza a senha 
 *  char adicionaSaldo(char valor[],char senha[]) retorna true se possivel adicionar (saldo total menor que 999,99) 
 *  char verificaCashBack(char indice,char valorVenda[])  retorna true se cashBack será aplicado 
 *  TAREFAS: Otimizar adiciona saldo e subtrai Saldo, verificar forma de entrada da nova senha e forma de entrada do valor a ser adicionado
 *  e cartão e etc.
 */
//==================================================================
#include "../Headers/cartoes.h"
#include "../Headers/lcd.h"




void atualizaSenha(char indice,char novaSenha[]){ // recebe o indice obtido por verificaContasLocais
	unsigned char i=0;
	for (i = 0;i<SENHASIZE;i++)
	{
		senhasLocais[indice-1][i]= novaSenha[i];
	}
}

char adicionaSaldo(char indice,char valorAdd[]){ // verificar como receberei o vetor do saldo a adicionar (se será igual a um valor de venda ou não ??)
	//fiz como se não fosse
	unsigned char k = indice -1,i;
	int centavos=0;
	int inteiros=0;
	int mul=0;
	// eventualmente criar uma função que converte de char  array para inteiros
	for (i =DIGITOS -1 ; i>=0; i--){
		switch(i){
			case 0:
			mul=1;
			break;
			case 1:
			mul=10;
			break;
			case 2:
			mul=1;
			break;
			case 3:
			mul=10;
			break;
			case 4:
			mul=100;
			break;
			default: mul=0;
			break;
		}
		if(i<2){
			centavos +=(int) (saldosLocais[k][i]+valorAdd[i]-96)*mul; //-96 para eliminar o efeito da ASCII

		}
		else {
			inteiros +=(int) (saldosLocais[k][i]+valorAdd[i]-96)*mul; //-96 para eliminar o efeito da ASCII
		}
	}
	if (centavos>= 100){
		inteiros = inteiros+1;
		centavos =centavos -100;
	}
	if(inteiros>=1000){
		return FALSE;
	}
	else{
	saldosLocais[k][4] = (char)(centavos%10) +48; //centesimos
	saldosLocais[k][3] = (char)(centavos/10) +48;  //decimos
	saldosLocais[k][0] = (char)(inteiros/100);
	inteiros = inteiros- saldosLocais[k][0]*100; 
	saldosLocais[k][0] =  saldosLocais[k][0]+48; //centenas
	saldosLocais[k][1] = (char)(inteiros/10)+48; //dezenas
	saldosLocais[k][2]	= (char)(inteiros%10)+48; // unidades
	}
}
	

char verificarContasLocais(char cartaoLido[]){ // verificada
	unsigned char i,j;
	for (i=0;i<LOCALCARDNUM;i++){
		for (j=0;j<CARDSIZE;j++){
			if(numCartoesLocais[i][j]!=cartaoLido[j]){
				break;
			}
			else if (j == CARDSIZE-1){ // todos os caracteres lido eram iguais
			 return (i+1); // retorno i+1 garante que encontrei um cara valido e já aproveita e passa o indice do cara +1
						   // externamentente não me preocupo com isso, se recebi um indice 1 eu irei trabalhar como 1 com ele fora 
						   // somente as funções desse bloco irão saber que tem que subtrair 1. 
			}
		}
	}
	return 0;
}

char verificaCashBack (char indice,char valorVenda[]){
	if(valorVenda[4]>=0 || valorVenda[3]>=5){
		if(cashBackCounter[indice-1] == 3){ // -1 para a correção do indice externo, conforme já foi comentado{
			
			return TRUE;
		}
		else {
				return FALSE;
		}
	}

}
//Verificada
char addCartao(char cardNum[],char senha[], char saldoInicial[]){ // retorna TRUE se criou com Sucesso e False  se criou com ERRO
	if(cartoesCadastrados<LOCALCARDNUM){
		carregaSaldo(saldoInicial);
		carregaNumCartao(cardNum);
		carregaSenha(senha);
		cashBackCounter[cartoesCadastrados] =0 ; //inicia sem cash back 
		cartoesCadastrados++; // cadastrei mais um cartão
		return TRUE;
	}
		return FALSE;
	
}

//Verificada
void carregaSaldo(char saldoInic[]){ // saldo inic deve estar em ASCII
	unsigned char i=0;
	for (i = 0;i<DIGITOS;i++)
	{
		saldosLocais[cartoesCadastrados][i]= saldoInic[i];
	}
}

//Verificada
void carregaNumCartao(char numCard[]){
	unsigned  char i=0;
	for (i = 0;i<CARDSIZE;i++)
	{
		numCartoesLocais[cartoesCadastrados][i]= numCard[i];
	}
}

//Verificada
void carregaSenha(char senha[]){ //senha deve estar em ASCII
	unsigned char i=0;
	for (i = 0;i<SENHASIZE;i++)
	{
		senhasLocais[cartoesCadastrados][i]= senha[i];
	}
}
// já terei o indice pois a ordem seria 
//verificarContasLocais
//	se Local ->	executarVendaInterna
//				verificaSenha
//				verificaCashBack
//				verificaSaldo
//					insuficiente -> retorna False 
//					suficiente -> subtrai
//									retorna true
char executarVendaInterna(char indice,char valor[], char senha[]){ // acho que como é algo interno não é interessante mandar uma mesagem, só dizer se é possivel
								// Retorno TRUE ou se e  falso// retorno FALSE 
	unsigned char auxiliar = verificaSenha(indice,senha);
	if (auxiliar){ //se senha valida
		auxiliar = verificaCashBack(indice,valor); // dessa forma é possivel comprar algo de 50 pila com 30 pila de saldo se  estou fazendo minha 3 compra, valoriza o cliente local
		if(auxiliar) {
			valor[3] = valor[3]-2;
			cashBackCounter[indice-1] = 0;
		} //20 reais de desconto
		else{
			cashBackCounter[indice-1]=cashBackCounter[indice-1] +1; // somente para deixar bem explicito
		}
		auxiliar = verificaSaldo(indice,valor);
		if (auxiliar) {
			subtraiSaldo(indice,valor);
			return 1;			// op realizada
		}
		else{
			return 0;		// erro sem saldo
		}
	
	}
	else{
		return 2;				// erro senha invalida
	}
	
}

void subtraiSaldo(char indice,char venda[]){
	//como já verifique é chars são numeros em ASCII -> posso só subtrailos e vou eliminar o offset
	// depois tenho que somar o  48 é 0 em ASCII
	unsigned char k = indice -1,i;
	int centavos=0;
	int inteiros=0;
	int mul=0;
	for (i =0 ; i< DIGITOS; i++){
		switch(i){
			case 0:
				mul=1;
				break;
			case 1:
				mul=10;
				break;
			case 2:
				mul=1;
				break;
			case 3:
				mul=10;
				break;
			case 4:
				mul=100;
				break;
			default: mul=0;
				break;
		}
		if(i<2){
			centavos +=(int) (saldosLocais[k][DIGITOS-1-i]-venda[i])*mul;

		}
		else {
			inteiros +=(int) (saldosLocais[k][DIGITOS-1-i]-venda[i])*mul;
		}
	}
	if (centavos< 0){
		inteiros = inteiros-1;
		centavos =centavos +100;
	}  
	 saldosLocais[k][4] = (char)(centavos%10) +48;
	 saldosLocais[k][3] = (char)(centavos/10) +48;
	 saldosLocais[k][0] = (char)(inteiros/100);
	 inteiros = inteiros- saldosLocais[k][0]*100;
	 
	 saldosLocais[k][0] =  saldosLocais[k][0]+48;
	 saldosLocais[k][1] = (char)(inteiros/10)+48;
	 saldosLocais[k][2]	= (char)(inteiros%10)+48;

}

char verificaSaldo (char indice, char valorVenda[]){ // Retorna TRUE ou FALSE de acordo com a disponibilidade de saldo
	unsigned char k = indice -1; // somente para as expressões ficarem mais claras, de certa forma é um disperdício de performace 
	// venda tem 5 caracteres e eles estão na ordem invertida. Ex: R$ 652,43 -> {3, 4 ,2, 5, 6}
	// Depois está muito hardcoded, poderia aprimorar isso em versoes futuras
	if      (saldosLocais[k][0] >  valorVenda[4]) {return TRUE;}  //centenas
	
	else if (saldosLocais[k][0] == valorVenda[4] && saldosLocais[k][1] >  valorVenda[3]) {return TRUE;} //dezenas
	
	else if (saldosLocais[k][0] == valorVenda[4] && saldosLocais[k][1] == valorVenda[3] && saldosLocais[k][2]  > valorVenda[2] ) {return TRUE;} //unidades
	
	else if (saldosLocais[k][0] == valorVenda[4] && saldosLocais[k][1] == valorVenda[3] && saldosLocais[k][2] == valorVenda[2] 
		  && saldosLocais[k][3]  > valorVenda[1]){return TRUE;} // decimos 
		  
	else if (saldosLocais[k][0] == valorVenda[4] && saldosLocais[k][1] == valorVenda[3] && saldosLocais[k][2] == valorVenda[2]
		  && saldosLocais[k][3] == valorVenda[1] && saldosLocais[k][4] >= valorVenda[0]) {return TRUE;} // centavos
		  
	else return FALSE;
}
	
 char verificaSenha(char indice, char senhaRecebida[]){
	unsigned char i;
	for (i =0;i<SENHASIZE;i++)
	{
		if(senhaRecebida[i] != senhasLocais[indice-1][i]){
			return FALSE;
		}
		else if (i==SENHASIZE-1){
			return TRUE;
		}
	}
	return TRUE;
}
void mostraSaldoNaSerial(char indice){  // função criada somente para DEBUG da máquina. 
	short k;
	for(k=0;k<cartoesCadastrados;k++){

		USART_envia(saldosLocais[k][0]);
		USART_envia(saldosLocais[k][1]);
		USART_envia(saldosLocais[k][2]);
		USART_envia(saldosLocais[k][3]);
		USART_envia(saldosLocais[k][4]);
		USART_envia(0x0D);
	}

}
