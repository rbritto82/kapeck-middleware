#include "Motores.h"

Motores::~Motores(){
	
}

Motores::Motores(){	
		
}

Motores::Motores(bool status_control){
	
	habilitaControlador(status_control);
}

void Motores::habilitaControlador(bool habilitado){
	unsigned char quadro_dados[TAMANHO_QUADRO];

	quadro_dados[PALAVRA_CONTR] = HAB_CONTROLADOR;

	if(habilitado){
		quadro_dados[1]=1;
	}
	else{
		quadro_dados[1]=0;
	}
	
	write_mod(TAMANHO_QUADRO, ID_MOTOR_DIR, quadro_dados);
	write_mod(TAMANHO_QUADRO, ID_MOTOR_ESQ, quadro_dados);
}

void Motores::refMotorDir(float ref){

	bool positivo =true;
	unsigned int ref_int;
	unsigned char quadro_dados[TAMANHO_QUADRO];
	

	if(ref >= 0)
		positivo =true;
	else
		positivo =false;

	ref = fabs(ref);

	if(ref > 1)
		ref = 1;

	ref = roundf(1023*ref);//valor normalizado de acordo com o pwm do hardware
	ref_int = (unsigned int)(int) ref;

	// transforma o inteiro em bytes para o envio pela rede
	quadro_dados[1] = (unsigned char)ref_int;
	ref_int >>=8;
	quadro_dados[0] = (unsigned char)ref_int;
	
	if(!positivo)
		quadro_dados[0] = quadro_dados[0]+ (unsigned char)DIRECAO_NEG;
	
	quadro_dados[0] = quadro_dados[0] + (unsigned char)CONTR_ACIONAR;

	write_mod(TAMANHO_QUADRO, ID_MOTOR_DIR, quadro_dados);
	
}

void Motores::refMotorEsq(float ref){

	bool positivo =true;
	unsigned int ref_int;
	unsigned char quadro_dados[TAMANHO_QUADRO];
	

	if(ref >= 0)
		positivo =true;
	else
		positivo =false;

	ref = fabs(ref);

	if(ref > 1)
		ref = 1;

	ref = roundf(1023*ref);
	ref_int = (unsigned int)(int) ref;

	quadro_dados[1] = (unsigned char)ref_int;
	ref_int >>=8;
	quadro_dados[0] = (unsigned char)ref_int;
	
	if(!positivo)
		quadro_dados[0] = quadro_dados[0]+ (unsigned char)DIRECAO_NEG;
	
	quadro_dados[0] = quadro_dados[0] + (unsigned char)CONTR_ACIONAR;

	write_mod(TAMANHO_QUADRO, ID_MOTOR_ESQ, quadro_dados);

}

unsigned int Motores::encoderEsq(){

	unsigned char quadro_dados[TAMANHO_QUADRO];
	unsigned char quadro_recepcao[8];
	unsigned int leitura_encoder=0;

	quadro_dados[0] = CONTR_ENCONDER;
	quadro_dados[1] = 0;
	quadro_dados[2] = 0;
	quadro_dados[3] = 0;
	quadro_dados[4] = 0;

	write_mod(TAMANHO_QUADRO, ID_MOTOR_ESQ, quadro_dados);

	read_mod(quadro_recepcao);
	
	leitura_encoder = quadro_recepcao[0];
	leitura_encoder <<=8;
	leitura_encoder = leitura_encoder + quadro_recepcao[1];	


	return leitura_encoder;
}

unsigned int Motores::encoderDir(){

	unsigned char quadro_dados[TAMANHO_QUADRO];
	unsigned char quadro_recepcao[8];
	unsigned int leitura_encoder=0;

	quadro_dados[0] = CONTR_ENCONDER;
	quadro_dados[1] = 0;
	quadro_dados[2] = 0;
	quadro_dados[3] = 0;
	quadro_dados[4] = 0;

	write_mod(TAMANHO_QUADRO, ID_MOTOR_DIR, quadro_dados);

	read_mod(quadro_recepcao);
	
	leitura_encoder = quadro_recepcao[0];
	leitura_encoder <<=8;
	leitura_encoder = leitura_encoder + quadro_recepcao[1];	


	return leitura_encoder;
}

float Motores::angDir(){
		
	return ((encoderDir()*2*M_PI)/(PULSOS_REVOL*REDUCAO_MOTOR));
	 
}

float Motores::angEsq(){
		
	return ((encoderEsq()*2*M_PI)/(PULSOS_REVOL*REDUCAO_MOTOR));
	 
}
