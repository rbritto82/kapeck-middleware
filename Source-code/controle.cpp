#include "controle.h"

PID::PID(){
	
	double k=0.2,ti=10,td=0.1,n=1E+6;  
    fixa_constantes(k,ti,td,n);
}
PID::PID(double k, double ti, double td, double n){
	fixa_constantes(k,ti,td,n);
}

void PID::fixa_constantes(double k, double ti, double td, double n){
	
	K=k; Ti=ti; Td=td; N=n;
	e_ant = I_ant = I_ant2 = D_ant = 0.0;
}
    
double PID::controle(double e, double h){
	
	double P = K*e;
	double I = I_ant + (K*h/Ti)*e;
	double D = (Td/(Td+N*h))*D_ant + (K*Td*N/(Td+N*h))*(e-e_ant);
	//  double D = (K*Td/h)*(e-e_ant);
	double u = P + D + I;
	I_ant2 = I_ant; I_ant = I;
	e_ant = e;
	D_ant = D;
	return u;
}

void PID::anti_windup(){
	I_ant = I_ant2;
}

void PID::reset(){
	I_ant = I_ant2 = 0.0;
	e_ant = 0.0;
	D_ant = 0.0;
}

Controle::Controle(bool orientacao,double amostragem):lin(20,10,0.0,1E+6),ang(1,100,0.0,1E+6){
  
	T_AMOSTR = amostragem;
   	controle_orientacao = orientacao;
   	chegou = false;
   
}

// Esta funcao calcula o "percentual de levada em conta" do controle
// angular (de orientacao). Quando o robo esta muito distante da sua
// referencia final, nao e necessesario ainda se preocupar com a
// orientacao final de chegada, e esta funco retorna 0.0. Este valor
// vai crescendo ate se tornar 1.0 para distancias pequenas.

inline double coef_orient(double d){
  return ( d<DIST_ORIENT ? 1.0 :
	   ( d>2*DIST_ORIENT ? 0.0 : (2*DIST_ORIENT - d)/DIST_ORIENT ) );
}

struct VELOCIDADES Controle::controle(PONTO pos, PONTO ref){
	
	static double distancia,beta,beta2,gama,xref,yref,
    erro_ang,erro_ang2,erro_lin,alpha_lin,alpha_ang;
  
  	// Erros nulos se proximo da referencia
  	distancia = hypot( ref.y-pos.y,ref.x-pos.x );
	
  	if (!chegou){
      	chegou = (distancia < EPSILON_L);
  	}
  	else{
    	chegou = (distancia < DELTA_L);
  	}
  	if (!chegou){
    	beta = arc_tang( ref.y-pos.y,ref.x-pos.x );
      	// Calculo da referencia "xref,yref" para o controle de posicao de
      	// forma a garantir o controle de orientacao
      	if (controle_orientacao){
			gama = coef_orient(distancia)*ang_equiv(beta - ref.ang);
			xref = pos.x+distancia*cos(beta+gama);
			yref = pos.y+distancia*sin(beta+gama);
			beta2 = arc_tang(yref-pos.y, xref-pos.x );
      	}
      	else{
			xref = ref.x;
			yref = ref.y;
			beta2 = beta;
      	}
      	erro_ang = ang_equiv(beta2 - pos.ang);
      	erro_lin = distancia*cos(erro_ang);
      	erro_ang2 = erro_ang;
		// Gera sinal de controle para o movimento angular
    alpha_ang = ang.controle(erro_ang2, T_AMOSTR);
	
    // Saturacao
    if (alpha_ang > 1.0) {
    	alpha_ang = 1.0;
      	ang.anti_windup(); // Podia dispensar, ja que e PD
    }
    if (alpha_ang < -1.0) {
    	alpha_ang = -1.0;
    	ang.anti_windup(); // Podia dispensar, ja que e PD
    }
    // Gera sinal de controle para o movimento linear
    alpha_lin = lin.controle(erro_lin, T_AMOSTR);
	
    // Saturacao
    if (alpha_lin > 1-fabs(alpha_ang)) {
    	alpha_lin = 1-fabs(alpha_ang);
    	lin.anti_windup(); // Podia dispensar, ja que e PD
    }
    if (alpha_lin < -(1-fabs(alpha_ang))) {
    	alpha_lin = -(1-fabs(alpha_ang));
    	lin.anti_windup(); // Podia dispensar, ja que e PD
    }

    // Calculo dos percentuais dos motores das rodas. Os valores
    // "alpha_ang" e "alpha_lin" sao  puramente tecnicos, pois o que se
    // controla na pratica sao os percentuais dos motores direito e
    // esquerdo.
    velocidades.vd = alpha_lin+alpha_ang;
    velocidades.ve = alpha_lin-alpha_ang;
	
	//Correção de "Zona Morta"
	if (velocidades.vd > 0) velocidades.vd = LIMIAR_ZM + ((1-LIMIAR_ZM)*velocidades.vd);
	if (velocidades.vd < 0) velocidades.vd = -LIMIAR_ZM + ((1-LIMIAR_ZM)*velocidades.vd);
	if (velocidades.ve > 0) velocidades.ve = LIMIAR_ZM + ((1-LIMIAR_ZM)*velocidades.ve);
	if (velocidades.ve < 0) velocidades.ve = -LIMIAR_ZM + ((1-LIMIAR_ZM)*velocidades.ve);
	  
    }
    else{
      suspender_controle();
	  //erro_ang2=0.0;
	  //erro_lin=0.0;
    }
    
  	
  	return velocidades;
}

bool Controle::suspender_controle(){
  
	chegou = false;
   	lin.reset();
   	ang.reset();
   	velocidades.vd = 0.0;
   	velocidades.ve = 0.0;
  
	return false;
}

//Funcao que calcula odometria a partir dos deslocamentos angulares das rodas
PONTO Controle::odometria(PONTO posicao, ENCODERS leitura){
	
	PONTO novaPosicao;	
	novaPosicao.x = posicao.x+(RAIO_RODA/2.0)*(leitura.encoder_D+leitura.encoder_E)*cos(posicao.ang);
	novaPosicao.y = posicao.y+(RAIO_RODA/2.0)*(leitura.encoder_D+leitura.encoder_E)*sin(posicao.ang);
	novaPosicao.ang = ang_equiv(posicao.ang+(RAIO_RODA/LADO_ROBO)*(leitura.encoder_D-leitura.encoder_E));
    
	return novaPosicao;
}

//Funcao que calcula o ponto alcancavel pelo robo em cada passo de amostragem
void Controle::setParamRef(PONTO xyinicial, PONTO xyfinal){
	
	float D=0.0;		
		
	deltaX = xyfinal.x - xyinicial.x;
	deltaY = xyfinal.y - xyinicial.y;
	D = hypot(deltaX,deltaY);
	N = round(D/(V_MAX_R*T_AMOSTR));	
	posIni.x = xyinicial.x;
	posIni.y = xyinicial.y;	
	
}

//Funcao que calcula o ponto alcancavel pelo robo em cada passo de amostragem
PONTO Controle::calcRef(int i){
		
	PONTO referencia;	
	if (i>N) i = N;
			
	referencia.x = posIni.x + (i/N)*deltaX;
	referencia.y = posIni.y + (i/N)*deltaY;
		
	return referencia;
}
