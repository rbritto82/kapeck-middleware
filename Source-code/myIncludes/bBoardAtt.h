/***************************************************************************
 *            bBoardAtt.h
 *
 *  Fri Dec  8 09:08:00 2006
 *  Copyright  2006  Ricardo de Sousa Britto
 *  Email  nephlinvocals@yahoo.com.br
 ****************************************************************************/

#ifndef _BBOARDATT_H
#define _BBOARDATT_H

#include <sys/errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "semaphore.h"

#define RW 0600 /* permissao de leitura e escrita */

template <class T>
class bBoardAtt{

  private:
      T *pmem;//ponteiro que aponta para área de memória
      int shmid;//identificador de área de memória
      semaphore s;//cria semáforo
  public:
	  bBoardAtt(int key); //criação de memória
  	  ~bBoardAtt();// destrutor
      void write(T dado);//escreve em área já criada
      T read(void);//ler de uma área já criada
};

template <class T>
bBoardAtt<T>::~bBoardAtt(){//destrutor
	
}

template <class T>
bBoardAtt<T>::bBoardAtt(int key){//construtor para se acoplar a memória criada
	                                                              
	shmid = shmget(key,sizeof(T),0);//recupera ID da memoria compartilhada 
		
	if (shmid == -1){
		perror("Erro na criacao do segmento de memoria") ;
		exit(1) ;
	}
	
	pmem = (T *) shmat(shmid, 0, 0);//acoplamento à área de memória
	
	if (s.connect_only(key))//acoplamento a semáforo mutex
    {
      perror( "Nao foi possivel se acoplar ao semaforo\n");
      exit(1);
    }
}

template <class T>
void bBoardAtt<T>::write(T dado){//método de leitura da memória compartilhada
   
	if (s.decrement())
	{
	  perror("Erro no decremento!\n");
	  exit(1);
	}
   
	*pmem = dado;//escreve no ponteiro que aponta pra área de memória
	
	if (s.increment())
	{
	  perror("Erro no incremento!\n");
	  exit(1);
	}
   	
}

template <class T>
T bBoardAtt<T>::read(void){//ler conteúdo da memória
   
   T dado;
   
   if (s.decrement())
	{
	  perror("Erro no decremento!\n");
	  exit(1);
	}
	
   dado = *pmem;
	
   if (s.increment())
	{
	  perror("Erro no incremento!\n");
	  exit(1);
	}
	
   return dado;//retorna o valor existente na área de memória	
}

#endif /* _BLACKBOARDACOPLAMENTO_H */
