#ifndef _CONTROLE_H_
#define _CONTROLE_H_

#include "dados.h"
#include "funcoes.h"
#include <iostream>

using namespace std;
//Para as distancias deve-se utilizar a unidade metro.
//Para os periodos de amostragem deve-se utilizar a unidade minutos.
class PID{
	
	private:
  		double K, Ti, Td, N;
  		double e_ant, I_ant, I_ant2, D_ant;
 	public:
  		// A relação com as constantes tradicionais do PID é a seguinte:
  		// Kp=k, Ki=k/ti, Kd=k*td
  		// Para eliminar a parte integral (Ki=0), ti=infinito
  		// Para eliminar a parte derivativa (Kd=0), td=0.0
  		// O número n é o limitador de altas freqüencias da parte derivativa.
  		// Normalmente, 3<=n<=20. Para eliminar a filtragem, n=infinito
  		PID(void);
  		PID(double k, double ti, double td, double n=1E+6);
  		void fixa_constantes(double k, double ti, double td, double n=1E+6);
  		void anti_windup(void);
  		void reset(void);
  		double controle(double erro, double h);
};

class Controle{
	
	private:
  		bool controle_orientacao;
  		bool chegou;
  		double T_AMOSTR;
  		PID lin,ang;
  		VELOCIDADES velocidades;
		float deltaX, deltaY;
		PONTO posIni;
		float N;
 	public:
  		Controle(bool orientacao,double amostragem);
  		struct VELOCIDADES controle(PONTO pos, PONTO ref);
  		bool suspender_controle();
		void setParamRef(PONTO xyinicial, PONTO xyfinal);
		PONTO calcRef(int i);
		PONTO odometria(PONTO posicao, ENCODERS leitura);
};

#endif
