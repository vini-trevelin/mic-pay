
//tipos de operações
void venda_avista();
void venda_parcelada();
void estorno();

// funções utilizadas pelas operações
void processa_pagamento(int parcelas,int tipo_cartao,char metodo_pgmt, char valor[],char senha[],char cartao[]);
int requisicao_externa(char cartao[],char senha[], char valor[], int parcelas);
uint8_t transforma_int(char valor[]);
void processa_venda(int opcao);
void processa_estorno(int tipo_cartao, char valor[],char cartao[]);
void formata_valor(char valor[]);
char metodo_pagamento();
int aguarda_cartao();

// recebimento de dados do usuario
void recebe_senha(char senha[]);
void recebe_cartao(char cartao[]);
void recebe_valor(char valor[]);