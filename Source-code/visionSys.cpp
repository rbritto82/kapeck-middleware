/***************************************************************************
 *            visionSys.cpp
 *
 *  Wed Sep  5 10:58:28 2007
 *  Copyright  2007  User Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/
 
#include "bBoardAtt.h"
#include "bBoardRe.h"
#include "dados.h" 
#include "funcoes.h"
#include "periodic.h"
#include <pthread.h>
#include <signal.h>
#include <iostream>

using  namespace::std;

bool simuCam(double angAbertura, double r, PONTO posCam,PONTO posMark);
bool status=true;
//sistema de visao
void* vision(void* x){
	long t0=0;
	periodic p1;
	PONTO mark = {0.8,2.0,0.0};
	PONTO robo = {0.0,0.0,0.0};
	bBoardAtt<VISAO> bVisao(BVISAO);
	bBoardRe<PONTO> bLocalizacao("10.13.99.227",BLOCALIZACAO);
	VISAO captura;
	captura.marca.x = mark.x;
	captura.marca.y = mark.y;
	captura.status = false;
	t0 = p1.setStart();
	while (status==true){	  
		  
		p1.setSleep(200,t0);
		robo = bLocalizacao.read(); 
		//robo.ang = robo.ang + (M_PI/4);
		captura.status = simuCam(M_PI/4,1,robo,mark);
		bVisao.write(captura);
	}  
	return NULL; 
}

int main(void){
	char status_tag;	
	pthread_t threadCo;
    pthread_create(&threadCo,NULL,vision,NULL);
	
	while(status == true){
		cout << "Para sair digite s:" << endl;
		cin >> status_tag;
		if (status_tag == 's'){
			status = false;
		}		
  	}
	
  	// Encerra as threads  	
  	pthread_kill(threadCo,SIGTERM);
  	pthread_join(threadCo,NULL);
	
  	return 0;
}

//------------------------Funcoes Auxiliares------------------
bool simuCam(double angAbertura, double r, PONTO posCam,PONTO posMark){
	bool status_cam=false;
	double deltaL=0.0, deltaX=0.0, deltaY=0.0, beta=0.0, abertura1=0.0, abertura2=0.0;
	
	deltaX = posMark.x - posCam.x;
	deltaY = posMark.y - posCam.y;
	deltaL = hypot(deltaX,deltaY);
	beta = atan2(deltaY,deltaX);
	abertura1 = posCam.ang - angAbertura;
	abertura2 = posCam.ang + angAbertura;	
	
	if((abertura1>=M_PI)||(abertura2>=M_PI)&&(beta<0)){
		beta=beta+2*M_PI;
	}
	if((abertura1<=-M_PI)||(abertura2<=-M_PI)&&(beta>0)){
		beta=beta-2*M_PI;
	}
	
	if((deltaL<=r)&&(abertura1<beta)&&(beta<abertura2)){
		status_cam = true;
		status = false;
		cout<<"achou"<<endl;
	}
	
	return status_cam;
}
