/***************************************************************************
 *            kapeck1.cpp
 *
 *  Tue Dec 12 15:10:18 2007
 *  Copyright  2007  User Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/
#include <iostream>
#include "bBoard.h"
#include "dados.h"

using namespace::std;

bool status = true;
char status_tag;

int main (void){

	bBoard <VISAO> bVisao(BVISAO);
			
	while(status == true){
		cout << "Para sair digite s:" << endl;
		cin >> status_tag;
		if (status_tag == 's'){
			status = false;
		}
	}
	
}
