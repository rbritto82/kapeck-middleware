/***************************************************************************
 *            bBoardServer.cpp
 *
 *  Tue Jan 11:30:18 2007
 *  Copyright  2007  User Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/
#include <iostream>
#include <string>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "semaphore.h"
#include "dados.h"
#include "ssocket.h"

using namespace std;

#define NUM_MAX_CONEX 10

// Os sockets dos clientes
tcpSocketServidor c;
semaphore sServBlack;
semaphore sBoard;
bool status = true;
int n = 0, j = 0, nconex = 0, cont = 0;

static void imprime_msg(SOCKET_STATUS err, const char *msg);

// Thread que efetivamente executa as transações com os clientes
// a cada novo cliente é criada uma thread serv_sockets
void *serv_sockets(void* x){
	unsigned char tipo_operacao;
	tcpSocket t = *((tcpSocket*)x);
	bool status = true;
	int cliente = 0;
	int tamanho=0, chaves = 0;
	void *pmem;//ponteiro que aponta para área de memória
    int shmid;//identificador de área de memória
	
	if (sServBlack.increment()){
	  	cerr<<"Erro no incremento!"<<endl;
	  	exit(1);
	}
    
	n = sizeof(chaves);
	cliente = ++cont;
		
	if ( t.read(&n,sizeof(n)) != SOCKET_OK ||t.read(&chaves,n) != SOCKET_OK ){
		cerr << "Problema na leitura do socket...\n";
		t.close();
		status = false;
    }
	if ( t.write(&n,sizeof(n)) != SOCKET_OK ||t.write(&chaves,n) != SOCKET_OK ){
		cerr << "Problema na leitura do socket...\n";
		t.close();
		status = false;
    }
	
	if ( t.read(&n,sizeof(n)) != SOCKET_OK ||t.read(&tamanho,n) != SOCKET_OK ){
		cerr << "Problema na leitura do socket...\n";
		t.close();
		status = false;
    }
	if ( t.write(&n,sizeof(n)) != SOCKET_OK ||t.write(&tamanho,n) != SOCKET_OK ){
		cerr << "Problema na leitura do socket...\n";
		t.close();
		status = false;
    }
	
	//recupera ID da memoria compartilhada
	shmid = shmget(chaves,tamanho,0);
	//acoplamento a semáforo mutex
	if (sBoard.connect_only(chaves)){
      perror( "Nao foi possivel se acoplar ao semaforo\n");
      exit(1);
    }
	
	if (shmid == -1){
		cerr<<"Erro na criacao do segmento de memoria"<<endl ;
		t.close();
		status = false;
	}	
	//acoplamento à área de memória
	pmem = shmat(shmid, 0, 0);
	
	while(status == true){     	
		
		n = sizeof(tipo_operacao);
		
	 	if ( t.read(&n,sizeof(n)) != SOCKET_OK ||t.read(&tipo_operacao,n) != SOCKET_OK ){
			
			cerr << "Problema na leitura do socket...\n";
			t.close();
			break;
     	}
		
		//se for uma operação de escrita
     	if(tipo_operacao == 0xAA){
						
	 		if ( t.write(&n,sizeof(n)) != SOCKET_OK || t.write(&tipo_operacao,n) != SOCKET_OK ) {
				cerr << "Problema na escrita no socket...\n";
				t.close();
				break;
    		}
			
			n = tamanho;
			
			//decrementa semaforo do blackboard
			if (sBoard.decrement()){
	  			cerr<<"Erro no decremento!"<<endl;
	  			exit(1);
			}
			
			if ( t.read(&n,sizeof(n)) != SOCKET_OK ||t.read(pmem,n) != SOCKET_OK ){
				cerr << "Problema na leitura do socket...\n";
				t.close();
				break;
     		}
			//Incrementa semaforo do blackboard
			if (sBoard.increment()){
	  			cerr<<"Erro no incremento!"<<endl;
	  			exit(1);
			}
								
			n = sizeof(tipo_operacao);
			
			if ( t.write(&n,sizeof(n)) != SOCKET_OK || t.write(&tipo_operacao,n) != SOCKET_OK ) {
				cerr << "Problema na escrita no socket...\n";
				t.close();
				break;
    		}
			/*cout << endl
	   		<< "=========================================================\n"
	   		<< "Dados Recebidos do Cliente "<< cliente << ": " << dado << endl
	   		<< "=========================================================\n";*/
			
	 	}
		//se for uma operação de leitura
		if(tipo_operacao == 0xAB){
			
	 		n = tamanho;
			
			//decrementa semaforo do blackboard
			if (sBoard.decrement()){
	  			cerr<<"Erro no decremento!"<<endl;
	  			exit(1);
			}
			
			if ( t.write(&n,sizeof(n)) != SOCKET_OK || t.write(pmem,n) != SOCKET_OK ) {
				cerr << "Problema na escrita no socket...\n";
				t.close();
				break;
    		}
			
			//Incrementa semaforo do blackboard
			if (sBoard.increment()){
	  			cerr<<"Erro no incremento!"<<endl;
	  			exit(1);
			}
			/*cout << endl
	   		<< "=========================================================\n"
	   		<< "Dados Enviados ao Cliente "<< cliente << ": " << dado << endl
	   		<< "=========================================================\n";*/
			
	 	}		 		
	 
  }
  t.close();
  return(NULL);	
}

// Thread que efetivamente desempenha as tarefas do servidor
void *servidor(void *x){
  	
	tcpSocket t;
  	pthread_t threadServ[NUM_MAX_CONEX];
		
	while (status == true) {
		//espera atividade no socket servidor 		
	  	if (c.accept(t) != SOCKET_OK){
	    	cerr << "Não foi possível estabelecer uma conexão\n";
	    	status = false;
	  		}		
			
	  	if (status == true){
			if (nconex >= NUM_MAX_CONEX){
	      		cerr << "Número máximo de conexões atingido: conexão rejeitada\n";
	      		t.close();
	    		}
	    		else{
	      			// Aloca socket livre para o novo cliente em um thread distinta
	      			pthread_create(&threadServ[nconex],NULL,serv_sockets,&t);
					if (sServBlack.decrement()){
	  					cerr<<"Erro no decremento!"<<endl;
	  					exit(1);
					}
					nconex++;					
	    		}
	  		}           
    		//cout << "\nSaiu\n";
	}
	//destroi todas as threads criadas para receber requisições dos clientes
	for (int i=0;i<nconex+1;i++){
		pthread_kill(threadServ[i],SIGTERM);
  		pthread_join(threadServ[i],NULL);
 	}
  	return(NULL);
}

int main(void){
	char status_tag;
  	socket_error = imprime_msg;
	
	//Acoplamento a semaforo sSimCAN ja criado em kapekOS.cpp
	if (sServBlack.connect_or_create(SSERVBLACKBOARD,0666,1)){
	  	cerr<<"Erro no acoplamento!"<<endl;
	  	exit(1);
	}
	
  	if (c.listen(PORTA_SERVBLACK,NUM_MAX_CONEX) != SOCKET_OK){
    	cerr << "Não foi possível abrir o socket de controle\n";
    	exit(1);
  	}
	
 	pthread_t tid;
	
  	// Cria a thread que escreve as mensagens recebidas
  	pthread_create(&tid,NULL,servidor,NULL);

  while (status == true) {
    cout << "Para sair digite s:" << endl;
		cin >> status_tag;
		if (status_tag == 's'){
			status = false;
		}
    
  }
  c.close();
  if (sServBlack.remove()){
		
	  cerr<<"Erro na remocao do semaforo!"<<endl;
	  exit(1);
	}
  sleep(1);
  pthread_kill(tid,SIGTERM);
  pthread_join(tid,NULL);  
  
}

//Funcao quetrata as mensagens de erro retornadas pela classe sssocket.
//Opcional, uma vez que a classe ssocket já possui esse mecanismo, mas faz o programa abortar.
static void imprime_msg(SOCKET_STATUS err, const char *msg){
	
  cerr << "Erro no programa servidor: " << msg << endl;
}
