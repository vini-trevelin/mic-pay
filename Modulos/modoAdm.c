

#include <avr/io.h>
#include "../Headers/lcd.h"
#include "../Headers/botoes.h"
#include "../Headers/modoAdm.h"
#include "../Headers/relogio.h"

static char statusOperadores[] = {1,1}; //dois operadores come�am habilitados

void modoADM(char *tecla){
	if((*tecla) == KEY_1){
		//mudar hora
		char dia[2],mes[2],ano[2],hora[2],min[2],seg[2],result;
		tela_instrucoes_configDiaMesAno();
		getDiaMesAno(dia,mes,ano); //preenche dia mes e ano
		tela_instrucoes_configHoraMinSeg();
		getHoraMinSeg(hora,min,seg);//preenche hora min e seg
		result = changeDate(dia,mes,ano,hora,min,seg); //faz o update
		if(result) //informa se update foi valido
			tela_operacaoConcluida();
		else
			tela_dataInvalida();
			
		*tecla = TECLA_INVALIDA;
	}else if((*tecla) == KEY_2){
		//ver pagamentos pendentes
	}else if((*tecla) == KEY_3){
		//hab/deshabilitar operadore
		tela_instrucoes_configOper();
		tela_status_oper();
		(*tecla) = TECLA_INVALIDA;
		while ((*tecla) == TECLA_INVALIDA){
			(*tecla) = teclaDebouce();
		}
		
		if((*tecla) == KEY_1 || (*tecla) == KEY_2 ) 
			mudarStatusOper(tecla); //passando o endere�o de tecla(teclaG da main), n�o o valor
		else{
			writeInstruction(lcd_Clear | lcd_Home);
			writeString("Opcao Invalida");	
			_delay_ms(1000);	
		}
		*tecla = TECLA_INVALIDA;
	}else if((*tecla) == KEY_4){
		//des pendencia
		*tecla = TECLA_INVALIDA;
	}
}

void mudarStatusOper(char *tecla){
	//muda o que tem no statusOperador baseado na tecla	
	int idx = (int)(*tecla);
	if (statusOperadores[idx] == 1)
		statusOperadores[idx]=0;
	else
		statusOperadores[idx]=1;
	tela_operacaoConcluida();
}

void tela_status_oper(){
	
	writeInstruction(lcd_Clear | lcd_Home);
	writeString("Oper1 ");
	if(statusOperadores[0]==1)
		writeString("deshab  -1");
	else
		writeString("hab     -1");
		
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Oper2 ");
	if(statusOperadores[1]==1)
		writeString("deshab  -2");
	else
		writeString("hab     -2");
}

char getOperStatus(char oper){
	return statusOperadores[(int)oper - 1];
}

void getDiaMesAno(char dia[], char mes[], char ano[]){
	char tecla = TECLA_INVALIDA;
	unsigned char cont=0;
	uint8_t offset = 6;
	writeString("  /  /  ");
	writeInstruction(lcd_SetCursor | (uint8_t)6 | lcd_LineTwo);
	while(cont<6){
		if(cont<2){
			tecla = teclaDebouce();
			if(tecla != TECLA_INVALIDA && tecla != KEY_APAGAR && tecla != KEY_CONFIRMA){
				if(tecla == KEY_0)
					tecla = -1;
				tecla += 49;
				writeCharacter(tecla);
				dia[cont] = tecla;
				cont++;
			}
			else if (tecla == KEY_APAGAR && cont>0){
				uint8_t aux = (cont%2 == 0 && cont!=0) ? lcd_SetCursor | ((uint8_t)(cont-2)+offset) : lcd_SetCursor | ((uint8_t)(cont-1)+offset) ;
				writeInstruction(lcd_LineTwo | aux);
				writeString(" "); //TROCAR POR CARACTERE DE ESPACO
				writeInstruction(lcd_LineTwo | aux);
				cont --;
			}
			tecla = TECLA_INVALIDA;
		}
		//cont = 0;
		//writeString("/");
		tecla = TECLA_INVALIDA;
		if(cont>=2 &&cont <4){
			if (cont ==2) writeInstruction(lcd_SetCursor | (uint8_t)(cont+1+offset) | lcd_LineTwo);
			tecla = teclaDebouce();
			if(tecla != TECLA_INVALIDA && tecla != KEY_APAGAR && tecla != KEY_CONFIRMA){
				if(tecla == KEY_0)
					tecla = -1;
				tecla +=49 ;
				writeCharacter(tecla);
				mes[cont-2] = tecla;
				cont++;
			}
			else if (tecla == KEY_APAGAR){
				uint8_t aux = (cont%2 == 0 && cont!=0) ? lcd_SetCursor | ((uint8_t)(cont-1)+offset) : lcd_SetCursor | ((uint8_t)(cont)+offset) ;
				writeInstruction(lcd_LineTwo | aux);
				writeString(" "); //TROCAR POR CARACTERE DE ESPACO
				writeInstruction(lcd_LineTwo | aux);
				cont --;
			}
			tecla = TECLA_INVALIDA;
		}
		//cont = 0;
		//writeString("/");
		if(cont>=4){
			if (cont ==4 ) writeInstruction(lcd_SetCursor | (uint8_t)(cont+2+offset)  | lcd_LineTwo);
			tecla = teclaDebouce();
			if(tecla != TECLA_INVALIDA && tecla != KEY_APAGAR && tecla != KEY_CONFIRMA){
				if(tecla == KEY_0)
					tecla = -1;
				tecla += 49;
				writeCharacter(tecla);
				ano[cont-4] = tecla;
				cont++;
			}
			else if (tecla == KEY_APAGAR){
				uint8_t aux = (cont%2 == 0) ? lcd_SetCursor | ((uint8_t)(cont)+offset) : lcd_SetCursor | ((uint8_t)(cont+1)+offset) ;
				writeInstruction(lcd_LineTwo | aux);
				writeString(" "); //TROCAR POR CARACTERE DE ESPACO
				writeInstruction(lcd_LineTwo | aux);
				cont --;
			}
			tecla = TECLA_INVALIDA;
		}
	}
	_delay_ms(750);
}


void getHoraMinSeg(char hora[], char min[], char seg[]){
	char tecla = TECLA_INVALIDA;
	unsigned char cont=0;
	writeString("  :  :  ");
	uint8_t offset = 6;
	//writeInstruction(lcd_SetCursor | lcd_LineTwo);
	writeInstruction(lcd_SetCursor | (uint8_t)6 | lcd_LineTwo);
	while (cont<6){
		if(cont<2){
			tecla = teclaDebouce();
			if(tecla != TECLA_INVALIDA && tecla != KEY_APAGAR && tecla != KEY_CONFIRMA){
				if(tecla == KEY_0)
					tecla = -1;
				tecla += 49;
				writeCharacter(tecla);
				hora[cont] = tecla;
				cont++;
			}
			else if (tecla == KEY_APAGAR && cont>0){
				uint8_t aux = (cont%2 == 0 && cont!=0) ? lcd_SetCursor | ((uint8_t)(cont-2)+offset) : lcd_SetCursor | ((uint8_t)(cont-1)+offset) ;
				writeInstruction(lcd_LineTwo | aux);
				writeString(" "); //TROCAR POR CARACTERE DE ESPACO
				writeInstruction(lcd_LineTwo | aux);
				cont --;
			}
			tecla = TECLA_INVALIDA;
		}
		//cont = 0;
		//writeString(":");
		tecla = TECLA_INVALIDA;
		if(cont>=2 &&cont <4){
			if (cont ==2) writeInstruction(lcd_SetCursor | (uint8_t)(cont+1+offset) | lcd_LineTwo);
			tecla = teclaDebouce();
			if(tecla != TECLA_INVALIDA && tecla != KEY_APAGAR && tecla != KEY_CONFIRMA){
				if(tecla == KEY_0)
					tecla = -1;
				tecla += 49;
				writeCharacter(tecla);
				min[cont] = tecla;
				cont++;
			}
			else if (tecla == KEY_APAGAR){
				uint8_t aux = (cont%2 == 0 && cont!=0) ? lcd_SetCursor | ((uint8_t)(cont-1)+offset) : lcd_SetCursor | ((uint8_t)(cont)+offset) ;
				writeInstruction(lcd_LineTwo | aux);
				writeString(" "); //TROCAR POR CARACTERE DE ESPACO
				writeInstruction(lcd_LineTwo | aux);
				cont --;
			}
			tecla = TECLA_INVALIDA;
		}
		//cont = 0;
		//writeString(":");
		if(cont>=4){
			if (cont == 4) writeInstruction(lcd_SetCursor | (uint8_t)(cont+2+offset) | lcd_LineTwo);
			tecla = teclaDebouce();
			if(tecla != TECLA_INVALIDA && tecla != KEY_APAGAR && tecla != KEY_CONFIRMA){
				if(tecla == KEY_0)
					tecla = -1;
				tecla += 49;
				writeCharacter(tecla);
				seg[cont] = tecla;
				cont++;
			}
			else if (tecla == KEY_APAGAR){
				uint8_t aux = (cont%2 == 0 && cont!=0) ? lcd_SetCursor | ((uint8_t)(cont)+offset) : lcd_SetCursor | ((uint8_t)(cont+1)+offset) ;
				writeInstruction(lcd_LineTwo | aux);
				writeString(" "); //TROCAR POR CARACTERE DE ESPACO
				writeInstruction(lcd_LineTwo | aux);
				cont --;
			}
			tecla = TECLA_INVALIDA;
		}
	}
	_delay_ms(750);
}
