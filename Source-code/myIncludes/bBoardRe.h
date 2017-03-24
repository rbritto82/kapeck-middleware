/***************************************************************************
 *            blackboardRemoto.h
 *
 *  Fri Dec  8 09:21:54 2006
 *  Copyright  2006  Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/

#ifndef _BBOARDRE_H
#define _BBOARDRE_H

#include <sys/errno.h>
#include <sys/types.h>
#include <iostream>
#include "ssocket.h"
#include "dados.h"
#include "semaphore.h"

using namespace::std;
	
template <class T>
class bBoardRe{

  private:
	unsigned char tipo_operacao;
	tcpSocket s;
  	int n;
      
  public:
	  bBoardRe(const char* ip, int key, uint16_t porta); 
  	  bBoardRe(const char* ip, int key);
  	  ~bBoardRe();// destrutor
      void write(T dado);//escreve em área já criada
      T read(void);//ler de uma área já criada
};

template <class T>
bBoardRe<T>::~bBoardRe(){//destrutor
	//Encerra socket
	s.close();
}

template <class T>
bBoardRe<T>::bBoardRe(const char* ip, int key, uint16_t porta){//construtor para se acoplar a 
	                                                              //a memória criada	
  	int size;                                                              //a memória criada
  	
	if (s.connect(ip,porta) != SOCKET_OK){
    	cerr << "Problema na conexão do socket...\n";
    	exit(1);
  	}
	
	n = sizeof(key);
	
	if ( s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&key,n) != SOCKET_OK ) {
			cerr << "Problema na escrita no socket...\n";
			s.close();
			exit(1);
    }
	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&key,n) != SOCKET_OK ) {
			cerr << "Problema na escrita no socket...\n";
			s.close();
			exit(1);
    }
	size = sizeof(T);
	if ( s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&size,n) != SOCKET_OK ) {
			cerr << "Problema na escrita no socket...\n";
			s.close();
			exit(1);
    }
	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&size,n) != SOCKET_OK ) {
			cerr << "Problema na escrita no socket...\n";
			s.close();
			exit(1);
    }
}

template <class T>
bBoardRe<T>::bBoardRe(const char* ip, int key){//construtor para se acoplar a 
	int size;                                                              //a memória criada
  	
	if (s.connect(ip,PORTA_SERVBLACK) != SOCKET_OK){
    	cerr << "Problema na conexão do socket...\n";
    	exit(1);
  	}
	
	n = sizeof(key);
	
	if ( s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&key,n) != SOCKET_OK ) {
			cerr << "Problema na escrita no socket...\n";
			s.close();
			exit(1);
    }
	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&key,n) != SOCKET_OK ) {
			cerr << "Problema na escrita no socket...\n";
			s.close();
			exit(1);
    }
	size = sizeof(T);
	if ( s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&size,n) != SOCKET_OK ) {
			cerr << "Problema na escrita no socket...\n";
			s.close();
			exit(1);
    }
	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&size,n) != SOCKET_OK ) {
			cerr << "Problema na escrita no socket...\n";
			s.close();
			exit(1);
    }
}

template <class T>
void bBoardRe<T>::write(T dado){//escrita na memória compartilhada remota
   	
	tipo_operacao = 0xAA;
	n = sizeof(tipo_operacao);
	
	if ( s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&tipo_operacao,n) != SOCKET_OK ) {
			cerr << "Problema na escrita no socket...\n";
			s.close();
			exit(1);
    }
	
	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&tipo_operacao,n) != SOCKET_OK ) {
			cerr << "Problema na escrita no socket...\n";
			s.close();
			exit(1);
    }
	
	n = sizeof(T);
	
    if ( s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&dado,n) != SOCKET_OK ) {
			cerr << "Problema na escrita no socket...\n";
			s.close();
			exit(1);
    }

	n = sizeof(tipo_operacao);

	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&tipo_operacao,n) != SOCKET_OK ) {
			cerr << "Problema na escrita no socket...\n";
			s.close();
			exit(1);
    }
		
}

template <class T>
T bBoardRe<T>::read(void){//leitura da memória compartilhada remota
   	
	T dado;
   	tipo_operacao = 0xAB;
	n = sizeof(tipo_operacao);
	
	if ( s.write(&n,sizeof(n)) != SOCKET_OK || s.write(&tipo_operacao,n) != SOCKET_OK ) {
			cerr << "Problema na escrita no socket...\n";
			s.close();
			exit(1);
    }	

	n = sizeof(T);

	if ( s.read(&n,sizeof(n)) != SOCKET_OK || s.read(&dado,n) != SOCKET_OK ) {
			cerr << "Problema na escrita no socket...\n";
			s.close();
			exit(1);
    }

   	return (dado);//retorna o valor existente na área de memória	
}


#endif /* _BBOARDRE_H */
