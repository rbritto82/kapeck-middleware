#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <math.h>
#include "dados.h"

using namespace std;

bool simuCam(double angAbertura, double r, PONTO posCam,PONTO posMark){
	bool status=false;
	double deltaL=0.0, deltaX=0.0, deltaY=0.0, beta=0.0, abertura1=0.0, abertura2=0.0;
	
	deltaX = posMark.x - posCam.x;
	deltaY = posMark.y - posCam.y;
	deltaL = hypot(deltaX,deltaY);
	beta = atan2(deltaY,deltaX);
	cout<<"L="<<deltaL<<endl;
	abertura1 = posCam.ang - angAbertura;
	abertura2 = posCam.ang + angAbertura;
	cout<<"A1="<<abertura1<<"A2="<<abertura2<<endl;
	
	if((abertura1>=M_PI)||(abertura2>=M_PI)&&(beta<0)){
		beta=beta+2*M_PI;
	}
	if((abertura1<=-M_PI)||(abertura2<=-M_PI)&&(beta>0)){
		beta=beta-2*M_PI;
	}
	cout<<"beta="<<beta<<endl;
	if((deltaL<=r)&&(abertura1<beta<abertura2)){
		status = true;
	}
	
	return status;
}

int main(int argc, char* argv[]){
	PONTO mark = {0.2,0.2,0.0};
	PONTO camera = {0.0,0.0,M_PI/2};
	bool status=false;
	
	if(argc > 1){
		mark.x = atof(argv[1]);
		mark.y = atof(argv[2]);
		mark.ang = atof(argv[3]);
	}
	else{
		mark.x = 0.8;
		mark.y = 0.2;
	}
	
	status = simuCam(M_PI/4,1,camera,mark);
	//cout<<"status="<<status<<endl;
	if(!status)
		cout<<"Nao achou marca"<<endl;
	else
		cout<<"Achou marca"<<endl;
	
	return 0;
}
