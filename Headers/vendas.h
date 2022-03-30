
void processa_venda(int opcao);
void formata_valor(char valor[]);
char metodo_pagamento();
int aguarda_cartao();
void venda_avista();
void processa_pagamento(int parcelas,int tipo_cartao,char metodo_pgmt, char valor[],char senha[],char cartao[]);
void requisicao_externa(char cartao[],char senha[], char valor[], int parcelas);
uint8_t transforma_int(char valor[]);

//talvez fazer modulo só de telas/mnenus para não poluir o código
void menu_vendas();
void menu_pagamento();
void menu_cartao();
void tela_OK();
void tela_CF();
void tela_SI();
void tela_SF();