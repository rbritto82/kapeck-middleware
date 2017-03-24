/***************************************************************************
 *            kapeck2.cpp
 *
 *  Tue Dec 12 11:30:18 2006
 *  Copyright  2006  User Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/
#include <iostream>
#include "bBoard.h"
#include "dados.h"
#include "semaphore.h"

using namespace::std;

bool status = true;
char status_tag;

int main (void){
		
	bBoard<CAMINHO> bCaminho(BCAMINHO);
	bBoard<bool> bFim(BFIM);
	bBoard<PONTO> bReferencia(BREFERENCIA); 
	bBoard<PONTO> bLocalizacao(BLOCALIZACAO);	                 
	semaphore sProCaminho;//semaforo que sincroniza o consumo de caminho gerado
		
	//Criacao de semaforo sProCaminho
	if (sProCaminho.connect_or_create(SPROCAMINHO,0666,1)){
		
      cerr<< "Semáforo não foi criado"<<endl;
      exit(1);
    }
	
	//Decremento do semaforo sProCaminho		
	if (sProCaminho.decrement()){
		
	  	cerr<<"Erro no decremento!"<<endl;
	  	exit(1);
	}
	
	//Laço para finalizar o processo 
	while(status == true){
		
		cout << "Para sair digite s:" << endl;
		cin >> status_tag;
		if (status_tag == 's'){
			status = false;
		}
	}
	
	//Destruição de semáforo sProCaminho
	if (sProCaminho.remove()){
		
	  cerr<<"Erro na remocao do semaforo!"<<endl;
	  exit(1);
	}
	
}
