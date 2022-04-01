#include <avr/io.h>
#include "../Headers/lcd.h"
#include "../Headers/botoes.h"
#include "../Headers/senha.h"
#include "../Headers/telas.h"



char senhas[3][N_CARACSENHA] =  {	{1,1,1,1,1,1}, //adm
									{2,2,2,2,2,2}, //op 1
									{3,3,3,3,3,3}, //op 2
													};

char senhaLida[N_CARACSENHA] =  {'F','F','F','F','F','F'}; 				

char validarSenha(char senha1[],char senha2[]){
	short i = 0;
	char result =1;
	for (i =0;i < N_CARACSENHA;i++){
		if (senha1[i] != senha2[i])
		{
			writeCharacter(65);
			result = 0; // se for falso da exit
		}
	}
	return result; // retorna Verdadeiro se for igual
}									

//tenho que passar por referencia pq pode se querer desligar a maquina nesse momento
void lerSenha(char *teclaG){ // comecei a pensar no ler senha e tals. N�o sei se passar tudo para inteiro � a melhor forma de montar a senha
	//tamb�m tem quest�o dos numeros de digitos na senha e a parte de apagar o caractere que n�o est� funcionando 100% (n�o estou limpando onde estou pagando no display)
	(*teclaG) = acao_tecla(teclaDebouce());// mantendo a global ??
	short contador = 0;
	char flagOutConfirma = 0;
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	while (!flagOutConfirma || contador ==0){
		flagOutConfirma = 0;
		(*teclaG) = teclaDebouce();
		if((*teclaG)!= TECLA_INVALIDA && (*teclaG)!=KEY_CONFIRMA && (*teclaG) !=KEY_APAGAR && contador < N_CARACSENHA){
			senhaLida[contador] = (*teclaG)+1; // fun��o debounce ta retornando n-1 pro valor da tecla. Eventualmente corrigiir isso ??
			writeCharacter(senhaLida[contador]+48); // para otimizar um pouco procurar se o # ou o * est�o definidos em ASCII
			// # � 35 e * � 42, mas acho q deixamos mostrando a senha por enquanto pra facilitar
			contador++;
		}
		else if((*teclaG) == KEY_APAGAR){
			if(contador>0){
				uint8_t aux = lcd_SetCursor | (uint8_t)(contador-1);
				writeInstruction(lcd_LineTwo | aux);
				writeString(" "); //TROCAR POR CARACTERE DE ESPACO
				writeInstruction(lcd_LineTwo | aux);
				contador --;
			}
			senhaLida[contador] = 'F';
		}
		else if ((*teclaG) == KEY_CONFIRMA){
			flagOutConfirma = 1;
		}
	}
	(*teclaG) = TECLA_INVALIDA; //limpa a teclaG
}

char login(char *teclaG){
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_Home);
	writeString("Insira a Senha"); //chamada de senha inicial
	char senhaValida = 0;
	char userIndex = -1;
	short i;
	// para testar mais rapido
// 	userIndex = 0;
// 	return userIndex;
	///
	while(!senhaValida){
		lerSenha(teclaG); //estou passando o local da memoria de teclaG aqui, cabe a lerSenha usar *teclaG
		for (i =0;i<3 ; i++){
			if(senhaValida == 0 ){
				senhaValida = validarSenha(senhaLida,senhas[i]);
			}
			if (senhaValida){
				userIndex = i; //0 -> adm , 1 e 2 vendedores
				break;
			}
		}
	}
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Bem Vindo");
	writeInstruction(lcd_SetCursor | lcd_LineTwo);
	
	if(userIndex == 0)
		writeString("Administrador");
	else if(userIndex == 1)
		writeString("Operador 1");
	else if(userIndex == 2)
		writeString("Operador 2");
	
	return userIndex;
	
}

char continuar_modo_atual(char *tecla){
	tela_continuar_modo_atual();
	*tecla = TECLA_INVALIDA;
	while((*tecla)!=KEY_CONFIRMA && (*tecla)!=KEY_APAGAR )
		*tecla = teclaDebouce();
	
	return !((*tecla)==KEY_CONFIRMA);
		
}


