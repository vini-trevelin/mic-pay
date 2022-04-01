#define JAN  1 //31 d
#define FEV  2 //28 d
#define MAR  3 //31 d
#define ABR  4 //30 d
#define MAI  5 //31 d
#define JUN  6 //30 d
#define JUL  7 //31 d
#define AGO  8 //31 d
#define SET  9 //30 d
#define OUT 10 //31 d
#define NOV 11 //30 d
#define DEZ 12 //31 d
#define ERRO_DATA 0 // qualquer problema na data retorna isso

//VARIÁVEIS QUE DEVEM SER ACESSADAS

//static char stringHORA[9]; sempre depois de uma chamada de atualiza hora ou atualiza datahOra
//static char stringDATA[9]; sempre depois de uma chamada de atualiza hora ou atualiza datahOra
// serão strings  de argumento de um writeString() da biblioteca lcd


void anoZero(); //so pra inicializar o relogio em uma dataPradroa
char changeDate(char ArrNovoDia[],char ArrNovoMes[],char ArrNovoAno[], char ArrNovaHora[],char ArrNovoMinuto[],char ArrNovoSegundo[]);// retorna 1 se foi possivel efetuar a troca se não ERRO_DATA (0)
//se achar que precisa saber qual elemento foi digitado errado podemos mudar 
//do jeito que está é facil fazer ERRO_DIA_OU_MES_OU_ANO -> primeira chama de alteração de data
// e depois ERRO_MINUTO_OU_SEGUNDO_OU_HORA

void updateDate(); // CHAMAR DENTRO DO INTERRUP DE 1 SEGUNDO PARA SEMPRE ATUALIZAR O RELOGIO;
void AtualizaStringHora(); // atualiza somente a string de hora no formato HH:MM:SS
void AtualizaStringData();// atualiza somente a string de data no formato DD/MM/YY
void AtualizaStringDataHora(); // chama as duas funções acima




void atualizaMesAno(); // função interna, não chamar externamente
