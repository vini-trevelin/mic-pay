//relogio de 23 horas
/*
 *
 *		VARIÁVEIS QUE DEVEM SER ACESSADAS
 *	static char stringHORA[9]; sempre depois de uma chamada de atualiza hora ou atualiza datahOra
 *	static char stringDATA[9]; sempre depois de uma chamada de atualiza hora ou atualiza datahOra
 *   Serão strings  de argumento de um writeString() da biblioteca lcd
 *  static char dataFUTURA[6]; sempre depois de uma chamada de sumDate
 *	static char stringDATAFUTURA[9]; // sempre depois de uma chamada de sumDate para teste 
 *----------------------------------------------------------------------------------
 * FUNÇÕES
 * void anoZero(); -> só pra inicializar o relogio em uma dataPradroa
 * char changeDate(char novoDia,char novoMes,char novoAno, char novaHora,char novoMinuto,char novoSegundo);// retorna 1 se foi possivel efetuar a troca se não ERRO_DATA (0)
 * Se achar que precisa saber qual elemento foi digitado errado podemos mudar 
 * do jeito que está é facil fazer ERRO_DIA_OU_MES_OU_ANO -> primeira chama de alteração de data
 * e depois ERRO_MINUTO_OU_SEGUNDO_OU_HORA
 * void updateDate(); // CHAMAR DENTRO DO INTERRUP DE 1 SEGUNDO PARA SEMPRE ATUALIZAR O RELOGIO;
 * void AtualizaStringHora(); // atualiza somente a string de hora no formato HH:MM:SS
 *void AtualizaStringData();// atualiza somente a string de data no formato DD/MM/YY
 *void AtualizaStringDataHora(); // chama as duas funções acima
 * char sumDate(char numMesesAdd) deve ser chamada pra calcular a data de uma parela futura
 *
 */
 #include "../Headers/relogio.h"
unsigned static char dia;
unsigned static char mes;
unsigned static char ano; // de 2000 (00) até (2099) (99)
// Relógio estilo 0 -  23 horas
// software preve anos bissextos
void anoZero(){
	horas	 = 0;
	minutos  = 0;
	segundos = 0;
	dia =	1;
	mes =	1;
	ano =	22;
	stringDATA[2] = '/';
	stringDATA[5] = '/';
	stringDATA[8] = '\0'; 
	stringDATAFUTURA[2] = '/';
	stringDATAFUTURA[5] = '/';
	stringDATAFUTURA[8] = '\0'; 
	stringHORA[2] = ':';
	stringHORA[5] = ':';
	stringHORA[8] =  '\0'; 
	}


char changeDate(char ArrNovoDia[],char ArrNovoMes[],char ArrNovoAno[], char ArrNovaHora[],char ArrNovoMinuto[],char ArrNovoSegundo[]){ // TRUE(1) SE FOI FEITO COM SUCESSO
	char novoDia = (ArrNovoDia[0]-48)*10 + ArrNovoDia[1] - 48;
	char novoMes = (ArrNovoMes[0]-48)*10 + ArrNovoMes[1] - 48;
	char novoAno = (ArrNovoAno[0]-48)*10 + ArrNovoAno[1] - 48;
	char novoMinuto  = (ArrNovoMinuto [0]-48)*10 + ArrNovoMinuto[1]  - 48;
	char novaHora    = (ArrNovaHora   [0]-48)*10 + ArrNovaHora [1]   - 48;
	char novoSegundo = (ArrNovoSegundo[0]-48)*10 + ArrNovoSegundo[1]- 48;
	if ((novoAno>99) || (novoMes>12) || (novoDia >31) ){
		return ERRO_DATA;
	}
	else{
		if(novoDia >28 && novoMes ==2){
			 if((novoAno%4)!=0 || novoDia!=29)return ERRO_DATA;
		}
		else if (novoDia == 31){
			switch(novoMes){
				// sem break todos os casos fazem a mesma coisa, só fica mais  legível de que com varias ORs
				case ABR:
				case JUN:
				case SET:
				case NOV:
					return ERRO_DATA;
					break;
				default:
					break;
			}
		}
	}
	if (novoSegundo >59 || novoMinuto >59|| novaHora>23){
		return ERRO_DATA;
	}
	// FIM DA VERIFICAÇÂO DA DATA 
	else{
		dia = novoDia;
		mes = novoMes;
		ano = novoAno;
		horas = novaHora;
		minutos = novoMinuto;
		segundos = novoSegundo;
		return 1;
	}
}

void updateDate(){
	segundos ++;
	if (segundos==60){
		segundos=0;
		minutos++;
	}
	if(minutos==60){
		minutos=0;
		horas ++;
	}
	if (horas==24){
		horas = 0;
		dia++;
			switch(mes){
				case JAN:
				case MAR:
				case MAI:
				case JUL:
				case AGO:
				case OUT:
				case DEZ:
				if(dia==32){
					atualizaMesAno();
				}
					break;
				case FEV:
					if( (dia ==29 && (ano%4)!=0) || dia ==30){
					atualizaMesAno();
					}
				break;
				default: if(dia==31){
					atualizaMesAno();
					}
				break;
				}
			}	
}

char sumDate(char numMesesAdd) {
	char diaFut = dia;
	char mesFut = mes;
	char anoFut = ano;
	char i =0;
	while(i < numMesesAdd){
		mesFut ++;
		if (mesFut >= 13) {
			mesFut = mesFut-12;
			anoFut = anoFut +1;
			if (anoFut> 99) return 0;
		}
		switch(mesFut){
			case ABR:
			case JUN:
			case SET:
			case NOV:
			if(diaFut==31){
				mesFut = mesFut+1;
				diaFut = 1;
			}
				break;
			case FEV:
				if(diaFut >28 ){
					if((ano%4)==0){
						diaFut = diaFut - 28 ;
					}
					else{
						diaFut = diaFut -27;
					}
					mesFut++;
				}
				break;
			default: 
				break;
		}
	i++;
	}
	dataFUTURA[0] = diaFut/10 + 48;
	dataFUTURA[1] = diaFut%10 + 48;
	dataFUTURA[2] = mesFut/10 + 48;
	dataFUTURA[3] = mesFut%10 + 48;
	dataFUTURA[4] = anoFut/10 + 48;
	dataFUTURA[5] = anoFut%10 + 48;
	
	stringDATAFUTURA[0] = dataFUTURA[0];
	stringDATAFUTURA[1] = dataFUTURA[1];
	stringDATAFUTURA[3] = dataFUTURA[2];
	stringDATAFUTURA[4] = dataFUTURA[3];
	stringDATAFUTURA[6] = dataFUTURA[4];
	stringDATAFUTURA[7] = dataFUTURA[5];
	return 1;
}	

void atualizaMesAno(){
	dia =1;
	mes++;
	if (mes == 13){
		mes = 1;
		ano++;
		if (ano==100){
			ano = 0;
		}
	}
}

void AtualizaStringHora(){
	stringHORA[0] = horas/10 + 48;
	stringHORA[1] = horas%10 + 48;
	stringHORA[3] = minutos/10 + 48;
	stringHORA[4] = minutos%10 + 48;
	stringHORA[6] = segundos/10 + 48;
	stringHORA[7] = segundos%10 + 48;
}

void AtualizaStringData(){
	stringDATA[0] = dia/10 + 48;
	stringDATA[1] = dia%10 + 48;
	stringDATA[3] = mes/10 + 48;
	stringDATA[4] = mes%10 + 48;
	stringDATA[6] = ano/10 + 48;
	stringDATA[7] = ano%10 + 48;
}

void AtualizaStringDataHora(){
	AtualizaStringData();
	AtualizaStringHora();
}
