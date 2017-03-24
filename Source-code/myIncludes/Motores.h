#ifndef _Motores_H_
#define _Motores_H_

#include <math.h>
#include "usbCAN.h"

#define ID_MOTOR_ESQ	0x02
#define ID_MOTOR_DIR	0x01
#define TAMANHO_QUADRO	5
#define PALAVRA_CONTR 	0
#define DIRECAO_NEG		0x04
#define CONTR_ACIONAR	0x08
#define CONTR_ENCONDER	0x10
#define HAB_CONTROLADOR 0X20
#define REDUCAO_MOTOR 	128
#define PULSOS_REVOL 	16
class Motores: virtual private usbCAN{

	private:
		unsigned int encoderEsq();
		unsigned int encoderDir();
		
	public:
		void habilitaControlador(bool habilitado);	
		void refMotorDir(float ref);
		void refMotorEsq(float ref);
		float angDir();
		float angEsq();
		Motores(bool status_control);
		Motores();
		~Motores();
};

#endif
/*
ref	-É a referência para o controle de velocidade
	ou o percentual de PWM. Aceita valores somente
	de 1 até -1
*/

/*
	-Retorna o valor do encoder (número de pulsos). O valor varia de 0 a 65.535

	obs.: Para o robô Karel a equação para conversão de pulsos de encoder 
	para distâcia percorrida por uma roda é:
		  U = Cm · N
	onde:
	U  - Distancia percorrida pela roda em metros.
	N  - Periodo de amostragem.
	Cm - Constante igual 0.000177157
*/
