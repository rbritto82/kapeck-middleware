/***************************************************************************
 *            dados.h
 *
 *  Tue Mar  6 14:52:03 2007
 *  Copyright  2007  User Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/

#ifndef _DADOS_H
#define _DADOS_H

//Chaves dos blackboards e semaforos utilizados na arquitetura
#define SARBCAN 1
#define BODOMETRIA 3
#define BSONARES 4
#define BVISAO 5
#define BVISAOREMOTO 6
#define BCAMINHO 7
#define SPROCAMINHO 93
#define BFIM 300
#define SSERVBLACKBOARD 115
#define BREFERENCIA 12
#define BLOCALIZACAO 13
#define SSERVSEM 14
#define SEXPLORACAO 15
#define SSIMULADORCAN 70
#define SARB 200
#define SUSB 201

//Porta do Servidor de Blackboard
#define  PORTA_SERVBLACK 17056
//Porta do Servidor de Semaphoro
//#define  PORTA_SERVSEM 17057
//Porta do Arbitro do Barramento
#define  PORTA_ARB 17058
//Dados do robo
#define LADO_ROBO 0.47
#define RAIO_RODA 0.079
#define LIMIAR_ZM 0.25
#define W_MAX_R 0.403
#define V_MAX_R 0.15
#define W_MAX_CENTRAL 1
#define W_MAX_PT 0.5

// Distancia na qual se considera que o robo atingiu o alvo
#define EPSILON_L 0.05
// Distancia na qual se considera que o robo nao mais esta no alvo
#define DELTA_L (2.0*EPSILON_L)
// Distancia a partir da qual o controle de orientacao comeca a atuar
#define DIST_ORIENT 0.1
// Distancia na qual se considera que as partes da cabeca estereo atingiram o alvo
#define EPSILON_ANG 0.1

typedef struct PONTO{
    float x;
    float y;
    float ang;
};

typedef struct CAMINHO{
	PONTO p1;
	PONTO p2;
	PONTO p3;
};

typedef struct VELOCIDADES{
		float ve;
		float vd;
};

typedef struct ENCODERS{
		float encoder_E;
		float encoder_D;
};

typedef struct REFCABECA{
		float central;
		float pamD;
		float pamE;
		float tiltD;
		float tiltE;
};

typedef struct VISAO{
		PONTO marca;
		bool status;
};

typedef struct AMBIENTE{
		unsigned amb;
		bool achou;
};

typedef enum{
        MOTORES_D_E = 1,
        MOTOR_D = 2,        
        MOTOR_E = 3,
        ENCODERS_D_E = 4,
        ENCODER_D = 5,
        ENCODER_E = 6,
		ERRO_BUS = 666,	
} ID_CAN;

#endif /* _DADOS_H */
