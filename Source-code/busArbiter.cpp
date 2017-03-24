/***************************************************************************
 *            busArbiter.cpp
 *
 *  Fri Aug  3 09:36:34 2007
 *  Copyright  2007  User Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/

#include <iostream>
#include <string.h>
#include <signal.h>
#include "dados.h"
#include "ssocketU.h"
#include "Motores.h"
#include "semaphore.h"
 
#define NUM_MAX_CONEX 10
#define PATH_S "transf"

using namespace std;

tcpSocketUServidor c;
semaphore sArb;
semaphore sUSB;
bool status = true;
char status_tag;
int n = 0, j = 0, nconex = 0, cont = 0;
ID_CAN id;
Motores motores(false);

static void imprime_msg(SOCKET_STATUS err, const char *msg);

//Thread que efetivamente executa as transações com os clientes
//a cada novo cliente é criada uma thread serv_sockets
void *serv_sockets(void* x){
	
	float dado;
	float leitura;
	ENCODERS dadoE;
	VELOCIDADES dadoV;
	tcpSocketU t = *((tcpSocketU*)x);
	bool status = true;
	int cliente = 0;
	
	//liberação do semaforo que sincroniza a criação de novas threads
	if (sArb.increment()){
	  	cerr<<"Erro no incremento!"<<endl;
	  	exit(1);
	}
	
	cliente = ++cont;
	
	while(status == true){     	
				
		n = sizeof(id);
		
		//recebendo a operação desejada pelo cliente
		if ( t.read(&n,sizeof(n)) != SOCKET_OK || t.read(&id,n) != SOCKET_OK ){
			cerr << "Problema na leitura do socket..."<<endl;
			t.close();
			status = false;
    	}		
		
		//se for uma operação de acionamento de motor
     	if(id == MOTORES_D_E){
			
			//confirmando a operação desejado pelo cliente
	 		if ( t.write(&n,sizeof(n)) != SOCKET_OK || t.write(&id,n) != SOCKET_OK ) {
				cerr << "Problema na leitura do socket..."<<endl;
				t.close();
				break;
    		}
			
			n = sizeof(dadoV);
			
			//recebendo as referências
			if ( t.read(&n,sizeof(n)) != SOCKET_OK ||t.read(&dadoV,n) != SOCKET_OK ){
				cerr << "Problema na leitura do socket..."<<endl;
				t.close();
				break;
     		}
			
			//decrementando o semáforo que coordena o acesso ao barramento
			if (sUSB.decrement()){
	  			cerr<<"Erro no decremento!"<<endl;
	  			exit(1);
			}
			
			//enviando referências para os motores do robô
			motores.refMotorDir(dadoV.vd);
			motores.refMotorEsq(dadoV.ve);	
			
			//incrementando o semáforo que coordena o acesso ao barramento
			if (sUSB.increment()){
	  			cerr<<"Erro no incremento!"<<endl;
	  			exit(1);
			}			
			
			n = sizeof(id);
			
			//confirmando que a operação deu certo
			if ( t.write(&n,sizeof(n)) != SOCKET_OK || t.write(&id,n) != SOCKET_OK ) {
				cerr << "Problema na leitura do socket..."<<endl;
				t.close();
				break;
    		}
			/*cout << endl
	   		<< "=========================================================\n"
	   		<< "Motor acionado "<< dadoV.vd << endl
	   		<< "=========================================================\n";*/
			
	 	}
		
		
     	//se for uma operação de acionamento do motor esquerdo
     	if(id == MOTOR_E){
			
			//confirmando a operação desejado pelo cliente
	 		if ( t.write(&n,sizeof(n)) != SOCKET_OK || t.write(&id,n) != SOCKET_OK ) {
				cerr << "Problema na leitura do socket..."<<endl;
				t.close();
				break;
    		}
			
			n = sizeof(dado);
			
			//recebendo as referências
			if ( t.read(&n,sizeof(n)) != SOCKET_OK ||t.read(&dado,n) != SOCKET_OK ){
				cerr << "Problema na leitura do socket..."<<endl;
				t.close();
				break;
     		}
			
			//decrementando o semáforo que coordena o acesso ao barramento
			if (sUSB.decrement()){
	  			cerr<<"Erro no decremento!"<<endl;
	  			exit(1);
			}
			
			//enviando referência para o motor
			motores.refMotorEsq(dado);	
			
			//incrementando o semáforo que coordena o acesso ao barramento
			if (sUSB.increment()){
	  			cerr<<"Erro no incremento!"<<endl;
	  			exit(1);
			}			
			
			n = sizeof(id);
			
			//confirmando que a operação deu certo
			if ( t.write(&n,sizeof(n)) != SOCKET_OK || t.write(&id,n) != SOCKET_OK ) {
				cerr << "Problema na leitura do socket..."<<endl;
				t.close();
				break;
    		}
			
			/*cout << endl
	   		<< "=========================================================\n"
	   		<< "Motor acionado "<< id << endl
	   		<< "=========================================================\n";*/
			
	 	}
		
		//se for uma operação de acionamento do motor direito
     	if(id == MOTOR_D){
			
			//confirmando a operação desejado pelo cliente
	 		if ( t.write(&n,sizeof(n)) != SOCKET_OK || t.write(&id,n) != SOCKET_OK ) {
				cerr << "Problema na leitura do socket..."<<endl;
				t.close();
				break;
    		}
			
			n = sizeof(dado);
			
			//recebendo as referências
			if ( t.read(&n,sizeof(n)) != SOCKET_OK ||t.read(&dado,n) != SOCKET_OK ){
				cerr << "Problema na leitura do socket..."<<endl;
				t.close();
				break;
     		}
			
			//decrementando o semáforo que coordena o acesso ao barramento
			if (sUSB.decrement()){
	  			cerr<<"Erro no decremento!"<<endl;
	  			exit(1);
			}
			
			//enviando referência para o motor
			motores.refMotorDir(dado);	
			
			//incrementando o semáforo que coordena o acesso ao barramento
			if (sUSB.increment()){
	  			cerr<<"Erro no incremento!"<<endl;
	  			exit(1);
			}			
			
			n = sizeof(id);
			
			//confirmando que a operação deu certo
			if ( t.write(&n,sizeof(n)) != SOCKET_OK || t.write(&id,n) != SOCKET_OK ) {
				cerr << "Problema na leitura do socket..."<<endl;
				t.close();
				break;
    		}
			
			/*cout << endl
	   		<< "=========================================================\n"
	   		<< "Motor acionado "<< id << endl
	   		<< "=========================================================\n";*/
			
	 	}
		
		
		//se for uma operação de leitura de encoder
		if(id == ENCODERS_D_E){
							
			n = sizeof(dadoE);
		
			//decrementando o semáforo que coordena o acesso ao barramento
			if (sUSB.decrement()){
	  			cerr<<"Erro no decremento!"<<endl;
	  			exit(1);
			}
			//cout<<"teste"<<endl;
			//recebendo leitura dos encoders
			dadoE.encoder_E = motores.angEsq();	
			dadoE.encoder_D = motores.angDir();
			
			//incrementando o semáforo que coordena o acesso ao barramento
			if (sUSB.increment()){
	  			cerr<<"Erro no incremento!"<<endl;
	  			exit(1);
			}	
			
			//retornando o valor lido dos encoders	
			if ( t.write(&n,sizeof(n)) != SOCKET_OK || t.write(&dadoE,n) != SOCKET_OK ){
				cout << "Problema na escrita no socket...\n";
				t.close();
				break;
    		}
			
			/*cout << endl
	   		<< "=========================================================\n"
	   		<< "Dado lido do encoder D"<< id << ": " << dadoE.encoder_D << endl
	   		<< "=========================================================\n";
			cout << endl
	   		<< "=========================================================\n"
	   		<< "Dado lido do encoder E"<< id << ": " << dadoE.encoder_E << endl
	   		<< "=========================================================\n";*/
			
		}		
	
		//se for uma operação de leitura do encoder esquerdo
		if(id == ENCODER_E){
							
			n = sizeof(leitura);
		
			//decrementando o semáforo que coordena o acesso ao barramento
			if (sUSB.decrement()){
	  			cerr<<"Erro no decremento!"<<endl;
	  			exit(1);
			}
			
			//recebendo leitura do encoder
			leitura = motores.angEsq();
			
			//incrementando o semáforo que coordena o acesso ao barramento
			if (sUSB.increment()){
	  			cerr<<"Erro no incremento!"<<endl;
	  			exit(1);
			}	
			
			//retornando o valor lido dos encoders	
			if ( t.write(&n,sizeof(n)) != SOCKET_OK || t.write(&leitura,n) != SOCKET_OK ){
				cout << "Problema na escrita no socket...\n";
				t.close();
				break;
    		}
			
			/*cout << endl
	   		<< "=========================================================\n"
	   		<< "Dado lido do encoder E"<< id << ": " << leitura << endl
	   		<< "=========================================================\n";
			*/
			
		}		
	 
		//se for uma operação de leitura do encoder direito
		if(id == ENCODER_D){
							
			n = sizeof(leitura);
		
			//decrementando o semáforo que coordena o acesso ao barramento
			if (sUSB.decrement()){
	  			cerr<<"Erro no decremento!"<<endl;
	  			exit(1);
			}
			
			//recebendo leitura do encoder
			leitura = motores.angDir();
			
			//incrementando o semáforo que coordena o acesso ao barramento
			if (sUSB.increment()){
	  			cerr<<"Erro no incremento!"<<endl;
	  			exit(1);
			}	
			
			//retornando o valor lido dos encoders	
			if ( t.write(&n,sizeof(n)) != SOCKET_OK || t.write(&leitura,n) != SOCKET_OK ){
				cout << "Problema na escrita no socket...\n";
				t.close();
				break;
    		}
			
			/*cout << endl
	   		<< "=========================================================\n"
	   		<< "Dado lido do encoder D"<< id << ": " << leitura << endl
	   		<< "=========================================================\n";
			*/
			
		}		
  }
  t.close();
  return(NULL);	
}

// Thread que efetivamente desempenha as tarefas do servidor
void *servidor(void *x){
  	
	tcpSocketU t;
  	pthread_t threadServ[NUM_MAX_CONEX];
	
	while (status == true){
		//espera atividade no socket servidor 		
	  	if (c.accept(t) != SOCKET_OK){
	    	cout << "Não foi possível estabelecer uma conexão\n";
	    	status = false;
	  		}		
			
	  	if (status == true){
	
	      		// Aloca socket livre para o novo cliente em um thread distinta
	      		pthread_create(&threadServ[nconex],NULL,serv_sockets,&t);
				if (sArb.decrement()){
	  				perror("Erro no decremento!\n");
	  				exit(1);
				}
				nconex++;				
	  	}           
    	
	}
	//destroi todas as threads criadas para receber requisições dos clientes
	for (int i=0;i<nconex+1;i++){
		pthread_kill(threadServ[i],SIGTERM);
  		pthread_join(threadServ[i],NULL);
 	}
  	return(NULL);
}

int main(void){
	
  	socketU_error = imprime_msg;	
  
	//Criação de semáforo que controla a criação das threads auxiliares 
	if (sArb.connect_or_create(SARB,0666,1)){
	  	cerr<<"Erro no acoplamento!"<<endl;
	  	exit(1);
	}
	
	//Criação de semáforo que controla o acesso ao barramento CAN
	if (sUSB.connect_or_create(SUSB,0666,1)){
	  	cerr<<"Erro no acoplamento!"<<endl;
	  	exit(1);
	}
	
	//Servidor fica esperando conexão dos clientes
  	if (c.listen(PATH_S,NUM_MAX_CONEX) != SOCKET_OK){
    	cout << "Não foi possível abrir o socket de controle\n";
    	exit(1);
  	}
  
  	pthread_t tid;
  	
	// Cria a thread que escreve as mensagens recebidas
  	pthread_create(&tid,NULL,servidor,NULL);
	//laço para finalização do árbitro
  	while (status == true) {
    	cout << "Para sair digite s:" << endl;
		cin >> status_tag;
		
		if (status_tag == 's'){
			status = false;
		}
    
  	}
  	//fecha socket servidor
	c.close();
	//destrói semáforo de criação de threads auxiliares 
	if (sArb.remove()){
		
	  cerr<<"Erro na remocao do semaforo!"<<endl;
	  exit(1);
	}
	//destrói semáforo que garante exclusão mútua do barramento
	if (sUSB.remove()){
		
	  cerr<<"Erro na remocao do semaforo!"<<endl;
	  exit(1);
	}
  	sleep(1);
	//destruição de todas as threads criadas 
  	pthread_kill(tid,SIGTERM);
  	pthread_join(tid,NULL);  
  
}

//--------------------Funcoes Auxiliares---------------------------
//função que trata as mensagens de erro retornadas pela classe sssocket.
//Opcional, uma vez que a classe ssocket já possui esse mecanismo, mas faz o programa
//abortar.
static void imprime_msg(SOCKET_STATUS err, const char *msg){
	
  cout << "Erro no programa servidor: " << msg << endl;
	
}
