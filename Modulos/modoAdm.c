

#include <avr/io.h>
#include "../Headers/lcd.h"
#include "../Headers/botoes.h"
#include "../Headers/modoAdm.h"

static char statusOperadores[2] = {1,1};

void modoADM(char *tecla){
	
	if((*tecla) == KEY_1){
		//mudar hora
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
			mudarStatusOper(tecla); //passando o endereço de tecla(teclaG da main), não o valor
		else{
			writeInstruction(lcd_Clear | lcd_Home);
			writeString("Opcao Invalida");	
			_delay_ms(1000);	
		}
		*tecla = TECLA_INVALIDA;
	}else if((*tecla) == KEY_4){
		//des pendencia
	}
	*tecla = TECLA_INVALIDA;
}

void mudarStatusOper(char *tecla){
	//muda o que tem no statusOperador baseado na tecla	
	int idx = (int)(*tecla);
	if (statusOperadores[idx] == 1)
		statusOperadores[idx]=0;
	else
		statusOperadores[idx]=1;
	
	writeInstruction(lcd_Clear);
	writeInstruction(lcd_LineOne | lcd_SetCursor);
	writeString("Operacao");
	writeInstruction(lcd_LineTwo | lcd_SetCursor);
	writeString("Concluida!");
	_delay_ms(1000);
	
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
