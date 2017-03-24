/***************************************************************************
 *            usbCAN.cpp
 *
 *  Fri Aug  3 09:36:34 2007
 *  Copyright  2007  User Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/

#ifndef _USBCAN_H_
#define _USBCAN_H_

#include  <stdio.h>
#include <stdlib.h>
#include "ftd2xx.h"

#define MAX_DEVICES 2

enum{	//tipos de transmis� existentestes no PIC
 	teste,
	wireless,
	cam
};

class usbCAN{

	private:
		FT_HANDLE ftHandle[MAX_DEVICES+1];
		FT_STATUS ftStatus, porta_aberta, escrita_feita, leitura_feita, dado_ok, baud_OK;
		FT_STATUS evento_OK;
		EVENT_HANDLE eh;
		static bool status_open;
		
	protected:
		void read_mod(unsigned char* recepcao);
		void write_mod(unsigned char word_length, unsigned char id, unsigned char* dado);
	public:
		usbCAN();
		~usbCAN();
		void open_mod();
		void close_mod();	
		
};
#endif

/*
	recepcao	- ponteiro que aponta para posicao da memoria onde o valor
			recebido na leitura será escrito. (Vetor com tamanho do protocolo)
	word_length	- tamanho do dado em bytes( pode ser de 1 a 8).
	id		- endereço CAN de quem receberá a mensagem.
	dado		- dado que será transmitido ( vetor ).
	*/
