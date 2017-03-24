/***************************************************************************
 *            periodic.cpp
 *
 *  Tue Aug  7 15:18:32 2007
 *  Copyright  2007  User Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/

#include "periodic.h"

periodic::~periodic(void){

}

periodic::periodic(void){
	
	j=0;
	g=0;
}

long periodic::agora(long t){
      
	struct timeval tvA;
    struct timezone tzA;
    long dif, now;
    
	gettimeofday(&tvA,&tzA);
    now=((tvA.tv_sec*DEZNA6)+tvA.tv_usec);
    dif = now-t;
	return dif;
}
//Sincroniza a thread com a grade de execução do Linux
long periodic::setStart(void){
	long t0=0;
	sleep(1);
	g=gettimeofday(&tv,&tz);
    t0=(tv.tv_sec*DEZNA6)+tv.tv_usec;
	return t0;
}
//Define o tempo que o processo deve dormir
//Faz um processo ou thread dormir o (tempo desejado)-(tempo decorrido)
long periodic::setSleep(long time, long t0){
	j++;
	tp=j*time*1000*1000;
    ts.tv_sec=0;
    sleept=(j*time*1000)-agora(t0)-18000;
    ts.tv_nsec=sleept*1000;
    tsr.tv_sec=0;
    tsr.tv_nsec=0;
    nanosleep(&ts, &tsr);
	
	return sleept;
}
