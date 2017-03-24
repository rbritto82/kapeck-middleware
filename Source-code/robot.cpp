/***************************************************************************
 *            robot.cpp
 *
 *  Fri Aug  3 09:52:21 2007
 *  Copyright  2007  User Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/

#include "robot.h"
//destrutor
robot::~robot(void){
	
	//Encerra socket
	s.close();
	
	//destrói semáforo que garante exclusão mútua do barramento
	if (sem.remove()){		
	  cerr<<"Erro na remocao do semaforo!"<<endl;
	  exit(1);
	}
}

void robot::close(void){
	if (sem.remove()){		
	  cerr<<"Erro na remocao do semaforo!"<<endl;
	  exit(1);
	}
}

//construtor que conecta um modulo de software ao arbitro
robot::robot(const char* ip, int key){
	
  	if (s.connect(ip) != SOCKET_OK){
    	cerr << "Problema na conexão do socket...\n";
		close();
    	exit(1);
  	}
	
	//Criação de semáforo que controla o acesso ao barramento CAN
	if (sem.connect_or_create(key,0666,1)){
	  	cerr<<"Erro no acoplamento!"<<endl;
		close();
	  	exit(1);
	}
	
}

//aciona os dois motores em uma mesma instrução
void robot::onEngines(VELOCIDADES refs){
    
	ID_CAN id = MOTORES_D_E;
	
	n = sizeof(id);
	
	if (sem.decrement()){
	  	cerr<<"Erro no decremento!\n";
	  	exit(1);
	}
	
	if ( s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&id,n) != SOCKET_OK ) {
			cout << "Problema na escrita no socket...\n";
			s.close();
			close();
			exit(1);
    }
	
	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&id,n) != SOCKET_OK ) {
			cout << "Problema na leitura no socket...\n";
			s.close();
			close();
			exit(1);
    }
	
	n = sizeof(refs);
	
	if (s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&refs,n) != SOCKET_OK){
			cout << "Problema na escrita no socket...\n";
			s.close();
			close();
			exit(1);
	}
	
	n = sizeof(id);
	
	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&id,n) != SOCKET_OK ) {
			cout << "Problema na leitura no socket...\n";
			s.close();
			close();
			exit(1);
    }
	
	if (sem.increment()){
	  	cerr<<"Erro no incremento!\n";
	  	exit(1);
	}
	
}

//aciona motor esquerdo
void robot::onEngine_L(float ref){
    
	ID_CAN id = MOTOR_E;
		
	n = sizeof(id);
	
	if (sem.decrement()){
	  	cerr<<"Erro no decremento!\n";
	  	exit(1);
	}
	
	if ( s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&id,n) != SOCKET_OK ) {
			cout << "Problema na escrita no socket...\n";
			s.close();
			close();
			exit(1);
    }
	
	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&id,n) != SOCKET_OK ) {
			cout << "Problema na leitura no socket...\n";
			s.close();
			close();
			exit(1);
    }
	
	n = sizeof(ref);
	
	if (s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&ref,n) != SOCKET_OK){
			cout << "Problema na escrita no socket...\n";
			s.close();
			close();
			exit(1);
	}
	
	n = sizeof(id);
	
	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&id,n) != SOCKET_OK ) {
			cout << "Problema na leitura no socket...\n";
			s.close();
			close();
			exit(1);
    }
	
	if (sem.increment()){
	  	cerr<<"Erro no incremento!\n";
	  	exit(1);
	}
	
}

//aciona o motor direito
void robot::onEngine_R(float ref){
    
	ID_CAN id = MOTOR_D;
		
	n = sizeof(id);
	
	if (sem.decrement()){
	  	cerr<<"Erro no decremento!\n";
	  	exit(1);
	}
	
	if ( s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&id,n) != SOCKET_OK ) {
			cout << "Problema na escrita no socket...\n";
			s.close();
			close();
			exit(1);
    }
	
	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&id,n) != SOCKET_OK ) {
			cout << "Problema na leitura no socket...\n";
			s.close();
			close();
			exit(1);
    }
	
	n = sizeof(ref);
	
	if (s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&ref,n) != SOCKET_OK){
			cout << "Problema na escrita no socket...\n";
			s.close();
			close();
			exit(1);
	}
	
	n = sizeof(id);
	
	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&id,n) != SOCKET_OK ) {
			cout << "Problema na leitura no socket...\n";
			s.close();
			close();
			exit(1);
    }
	
	if (sem.increment()){
	  	cerr<<"Erro no incremento!\n";
	  	exit(1);
	}
	
}

//requisita leitura dos encoders em uma única instrução
struct ENCODERS robot::readEncoders(void){
	
	ID_CAN id = ENCODERS_D_E;
	ENCODERS leitura;	
	
	n = sizeof(id);
	
	if (sem.decrement()){
	  	cerr<<"Erro no decremento!\n";
	  	exit(1);
	}
	
	if ( s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&id,n) != SOCKET_OK ) {
			cout << "Problema na escrita no socket...\n";
			s.close();
			close();
			exit(1);
    }
	
	n = sizeof(leitura);
	
	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&leitura,n) != SOCKET_OK ) {
			cout << "Problema na leitura no socket...\n";
			s.close();
			close();
			exit(1);
    }
	
	if (sem.increment()){
	  	cerr<<"Erro no incremento!\n";
	  	exit(1);
	}	
		
	return (leitura);
}

//requisita leitura do encoder esquerdo
float robot::readEncoder_L(void){
	
	ID_CAN id = ENCODER_E;
	float leitura;
		
	n = sizeof(id);
	
	if (sem.decrement()){
	  	cerr<<"Erro no decremento!\n";
	  	exit(1);
	}
	
	if ( s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&id,n) != SOCKET_OK ) {
			cout << "Problema na escrita no socket...\n";
			s.close();
			close();
			exit(1);
    }
	
	n = sizeof(leitura);
	
	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&leitura,n) != SOCKET_OK ) {
			cout << "Problema na leitura no socket...\n";
			s.close();
			close();
			exit(1);
    }

	if (sem.increment()){
	  	cerr<<"Erro no incremento!\n";
	  	exit(1);
	}		
		
	return (leitura);
}

//requisita leitura do encoder direito
float robot::readEncoder_R(void){
	
	ID_CAN id = ENCODER_D;
	float leitura;
		
	n = sizeof(id);
	
	if (sem.decrement()){
	  	cerr<<"Erro no decremento!\n";
	  	exit(1);
	}
	
	if ( s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&id,n) != SOCKET_OK ) {
			cout << "Problema na escrita no socket...\n";
			s.close();
			close();
			exit(1);
    }
	
	n = sizeof(leitura);
	
	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&leitura,n) != SOCKET_OK ) {
			cout << "Problema na leitura no socket...\n";
			s.close();
			close();
			exit(1);
    }

	if (sem.increment()){
	  	cerr<<"Erro no incremento!\n";
	  	exit(1);
	}		
		
	return (leitura);
}
