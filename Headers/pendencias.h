#define NUMPAGSAGENDADOS 6 //NUM de pag que podem estar agendados
#define NUMDIGITOSCARTOES 6 //num de digitos em um cartão
#define NUMDIGITOSVALORES 5 //num de digitos em val de parcelas
#define NUMDATAS 12 //2*6 (max duas datas, cada uma com 6 digitos cada)

#define CARTAONAOENCONTRADO 0
#define PARCELAPAGA_OUPENDENTE 8
#define SEMPARCELA 9

void addPagamentoAgendado(char cartao[], char valParcela[], char datasVenci[]); //para add no caso de compra parcelada, uso externo
short removePagamentoAgendado(char cartao[]); //para remover quando o pagamento for recebido (12,18 ou 22)(sempre remove uma parcela, verifica qual deve ser), uso interno
void removePrimeiraParcela(short remover); //uso interno
void removeSegundaParcela(short remover); //uso interno
void organizaPagamentos(short remover); //para organizar dps q um foi removido, uso interno

char existePendencia(); //olha se tem alguma pendecia, retorna se tem (as 12, 18 e 22) (func pra deixar led ligada na pratica), uso externo
char getNumPendecias(); //retorna num de pendencias no sis, pro modo.
void verificarPendencia(); //add um pag agendado que expirou em pendencias, rodar as 22h sempre, uso externo
void addPendencias(short achada, char PouS); //para add pendencia, uso interno
char removePendencias(char remover); //pro adm remover pendencias manualmente (vai receber o indice dela)
void organizaPendencias(); //pra organizar dps q uma for removida

void cobrarPagementosAgendados(); //chama no interrupt do timer nos horarios certos, uso externo
void enviarPedidoDePagamento(short cobrar); //prepara o pedido de pag com quem deve pagar no dia, uso interno
char enviarPedidoSerial(char pedido[]); //envia o pedido pra serial, uso interno

char printPendencias(char sobeDesce); //printa a tela de pendencias pro adm escolher, uso externo
void printNumPendencia(short num); //printa a linha num de pendencias, uso interno

short comparaCartoes(char cartao1[],char cartao2[]);//compara dois cartoes
void testa_pendencia();