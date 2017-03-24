/***************************************************************************
 *            bBoard.h
 *
 *  Fri Nov 23 13:44:30 2006
 *  Copyright  2006  Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/

#ifndef _BBOARD_H
#define _BBOARD_H

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
class bBoard{

  private:
      T *pmem;//ponteiro que aponta para área de memória
      int shmid;//identificador de área de memória
      semaphore s;//cria semáforo
  public:
	  int size;
	  bBoard(int key); //criação de memória
  	  ~bBoard();// destrutor
      void write(T dado);//escreve em área já criada
      T read(void);//ler de uma área já criada
};

template <class T>
bBoard<T>::~bBoard(){//destrutor
	if (shmctl(shmid,IPC_RMID,0)==-1){
    perror("Impossivel de destruir o segmento de memoria") ;
    exit(1) ;
	}
	if (s.remove())
	{
	  perror("Erro na remocao do semaforo!\n");
	  exit(1);
	}
	
}

template <class T>
bBoard<T>::bBoard(int key){//construtor para criar memoria
	
	shmid = shmget(key,sizeof(T),RW|IPC_CREAT);// criacao da memoria compartilhada
		
	if (shmid == -1){
		perror("Erro na criacao do segmento de memoria") ;
		exit(1);
	}
	
	pmem = (T *) shmat(shmid, 0, 0);//acoplamento à área de memória
	
	size = sizeof(T);
	
	if (s.connect_or_create(key,0666,1))//criação de semáforo mutex
    {
      perror( "Semáforo não foi criado\n");
      exit(1);
    }
}

template <class T>
void bBoard<T>::write(T dado){//método de leitura da memória compartilhada
   
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
T bBoard<T>::read(void){//ler conteúdo da memória
   
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
#endif /* _BBOARD_H */
