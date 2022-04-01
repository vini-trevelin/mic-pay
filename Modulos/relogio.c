//relogio de 23 horas
/*
 *
 *		VARIÁVEIS QUE DEVEM SER ACESSADAS
 *	static char stringHORA[9]; sempre depois de uma chamada de atualiza hora ou atualiza datahOra
 *	static char stringDATA[9]; sempre depois de uma chamada de atualiza hora ou atualiza datahOra
 *   Serão strings  de argumento de um writeString() da biblioteca lcd
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
 *
 */
 #include "../Headers/relogio.h"
unsigned static char horas;
unsigned static char minutos;
unsigned static char segundos;
unsigned static char dia;
unsigned static char mes;
unsigned static char ano; // de 2000 (00) até (2099) (99)
static char stringHORA[9];
static char stringDATA[9];
// Relógio estilo 0 -  23 horas
// software não preve anos bissextos
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
	stringHORA[2] = ':';
	stringHORA[5] = ':';
	stringHORA[8] =  '\0'; 
	}
//talvez quebrar em duas funções ??


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
				// sem break todos os casos fazem a mesma coisa, só fica masi  legível de que com um mando de or 
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
					if( (dia ==29 && (ano%4)!=0) || dia ==30){// o maior é somente para segurança
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
