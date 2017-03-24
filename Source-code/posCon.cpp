/***************************************************************************
 *            posCon.cpp
 *
 *  Wed Sep  5 10:58:28 2007
 *  Copyright  2007  User Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/
 
#include "bBoardAtt.h"
#include "dados.h" 
#include "funcoes.h"
#include "controle.h"
#include "robot.h"
#include "periodic.h"
#include "semaphore.h"
#include <pthread.h>
#include <signal.h>
#include <iostream>
#include "xfig.h"
#include <fstream>

#define PATH_S "transf"
using  namespace::std;

float deltaL=0.0;
bool status = true;
char status_tag; 
bBoardAtt<PONTO> bReferencia(BREFERENCIA);
bBoardAtt<PONTO> bLocalizacao(BLOCALIZACAO);
bBoardAtt<bool> bFim(BFIM);
robot barramento(PATH_S,559);
semaphore sProCaminho;
XFig a(XF_PORTRAIT);
int id = 0, id2 = 0;
bool testaRegiao(PONTO xyatual, PONTO xyfinal, float erro);

//Controlador
void* controlePosicao(void* x){
	
	//------------Variáveis----------------------
	Controle c(false,0.2);	
	ENCODERS leitura1={0.0,0.0}, leitura0={0.0,0.0}, delta={0.0,0.0};
	PONTO referencia, referenciaM;
	VELOCIDADES velocidades={0.0,0.0}, velo_parar={0.0,0.0};
	PONTO posicao, novaPosicao;
	periodic p1,p2;
	int i=0,j=1;
	long t0_C=0;
	//Variaveis usadas para plotar tempo gasto em cada execucao do laco de controle
	int z=0;
	long sleept[500];
	struct timeval tvSleep1,tvSleep2;
    struct timezone tzSleep1,tzSleep2;
	long difSleep, nowSleep1, nowSleep2;
	FILE *pFileSleep, *pFileSleepD;
	
	//Variaveis usadas para plotar tempo gasto na leitura dos encoders
	//int z=0;
	long encoderst[500];
	struct timeval tvEncoders1,tvEncoders2;
    struct timezone tzEncoders1,tzEncoders2;
	long difEncoders, nowEncoders1, nowEncoders2;
	FILE *pFileEncoders;
	
	//Variaveis usadas para plotar tempo gasto na leitura dos encoders
	//int z=0;
	long motorest[500];
	struct timeval tvMotores1,tvMotores2;
    struct timezone tzMotores1,tzMotores2;
	long difMotores, nowMotores1, nowMotores2;
	FILE *pFileMotores;
	
	//Ler primeira referencia	
	referencia = bReferencia.read();
	//Decrementa semaforo que sincroniza o posCon e o pathPl
	if (sProCaminho.increment()){
	  	cerr<<"Erro no decremento!"<<endl;
	  	exit(1);
	}
	//Captura ponto de referencia para plot	
	a[id2].addLinearPoint(referencia.x*10,referencia.y*10);
	//Calcula valores bases para calcula de referencia movel
	c.setParamRef(bLocalizacao.read(), referencia);
	//Seta momento base para calcular os sleeps necessarios para se cumprir o deadline do Laco
	t0_C = p1.setStart();
	while (status==true){	
		//Seta momento base para captura de tempo consumido pelo laco de controle
		gettimeofday(&tvSleep1,&tzSleep1);
    	nowSleep1=((tvSleep1.tv_sec*1000000)+tvSleep1.tv_usec);
		
		//Faz o laco de controle dormir o necessario para comprir os 200 ms
		p1.setSleep(200,t0_C);
		
		//Seta momento base para captura de tempo gasto na leitura dos encoders
		gettimeofday(&tvEncoders1,&tzEncoders1);
    	nowEncoders1=((tvEncoders1.tv_sec*1000000)+tvEncoders1.tv_usec);
		
		//Ler medição dos encoders
		leitura1 = barramento.readEncoders();
		
		//Seta momento final para captura de tempo gasto na leitura dos encoders
		gettimeofday(&tvEncoders2,&tzEncoders2);
    	nowEncoders2=((tvEncoders2.tv_sec*1000000)+tvEncoders2.tv_usec);
		difEncoders = nowEncoders2 - nowEncoders1;
		encoderst[z] = difEncoders;
		
		//Testa se é a primeira leitura de encoder
		//Em caso positivo, iguala-se os valores 
		//da leitura anterior ao da leitura atual
		//de forma a o deslocamento ser zero
		if(i==0){
			leitura0.encoder_D=leitura1.encoder_D;
			leitura0.encoder_E=leitura1.encoder_E;
		}
		i++;
		
		//Calcula o deslocamento absoluto
		delta.encoder_E = ang_equiv(leitura1.encoder_E-leitura0.encoder_E);
		delta.encoder_D = ang_equiv(leitura1.encoder_D-leitura0.encoder_D);
		
		//Atualiza a variável que guarda o valor da leitura anterior
		leitura0.encoder_D=leitura1.encoder_D;
		leitura0.encoder_E=leitura1.encoder_E;
		
		//Lê a posição anterior do robô
		posicao = bLocalizacao.read();
		
		//Atualiza a nova posição resultado do último sinal de controle
		novaPosicao = c.odometria(posicao,delta);
		
		if((posicao.x!=novaPosicao.x)&&(posicao.y!=novaPosicao.y))
			a[id].addLinearPoint(novaPosicao.x*10,novaPosicao.y*10);
		
		//Testa se já chegou a região correta
		if(testaRegiao(novaPosicao,referencia,EPSILON_L)){
			if(bFim.read()==true){ 
				status=false;
				break;
			}
			else{
				if (sProCaminho.increment()){
	  				cerr<<"Erro no decremento!"<<endl;
	  				exit(1);
				}			
				j=1;
				referencia = bReferencia.read();
				cout<<"X= "<<novaPosicao.x<<" Y= "<<novaPosicao.y<<" Ang= "<<novaPosicao.ang<<endl;			
				c.setParamRef(novaPosicao, referencia);
				a[id2].addLinearPoint(referencia.x*10,referencia.y*10);
			}			
		}	
		
		//Atualiza a posição do robô
		bLocalizacao.write(novaPosicao);
		
		//Calcula a referência intermediária 
		referenciaM = c.calcRef(j);
		j++;
		
		//Calcula sinal de controle para os motores		
		velocidades = c.controle(novaPosicao,referenciaM);
		
		//Seta momento base para captura de tempo gasto no acionamento dos motores
		gettimeofday(&tvMotores1,&tzMotores1);
    	nowMotores1=((tvMotores1.tv_sec*1000000)+tvMotores1.tv_usec);
		
		//Envia sinal de controle para os motores		
		barramento.onEngines(velocidades);
		
		gettimeofday(&tvMotores2,&tzMotores2);
		nowMotores2=((tvMotores2.tv_sec*1000000)+tvMotores2.tv_usec);
		difMotores = nowMotores2 - nowMotores1;
		motorest[z] = difMotores;
		
		//Guarda tempo real do periodo de amostragem
		gettimeofday(&tvSleep2,&tzSleep2);
    	nowSleep2=((tvSleep2.tv_sec*1000000)+tvSleep2.tv_usec);
		difSleep = nowSleep2 - nowSleep1;
		sleept[z] = difSleep;
		z++;
	}
	barramento.onEngines(velo_parar);
	//Salva em arquivo xfig o Caminho Percorrido e o Caminho Planejado
	a.save("mapa_real.fig");
	
	//Cria arquivos que receberão os tempos adquiridos
	pFileSleep = fopen("sleep", "w");
	pFileSleepD = fopen("sleepD", "w");
	pFileEncoders = fopen("encoders", "w");
	pFileMotores = fopen("motores", "w");
	
	//Salva em arquivo os tempos adquiridos
	for(int h=0;h<z;h++){
		fprintf(pFileSleep, "%i %lf \n", h, sleept[h]/1000.0);
		fprintf(pFileSleepD, "%i %d \n", h, 200);
		fprintf(pFileEncoders, "%i %lf \n", h, encoderst[h]/1000.0);
		fprintf(pFileMotores, "%i %lf \n", h, motorest[h]/1000.0);
	}
	cout<<"Chegou..."<<endl;
	cout << "Digite s para sair:" << endl; 
	return NULL; 
}

int main(void){
	
	//-----------Configuração do arquivo xfig do mapa a ser gerado--------------
	
	// Na hora de exportar, a figura será aumentada de 25%
  	a.setScale(100.0);
  	// Cada unidade das dimensoes do programa correspondera a 1.0cm no xfig
  	a.setUnit(1.0);
	//Sala01
	id = a.addBox(0.0,37.9,13.4,56);
  	a[id].setDepth(100);
  	//Corredor
	id = a.addBox(0.0,0.0, 13.4,37.9);
  	a[id].setDepth(100);
  	//Sala02
	id = a.addBox(13.4,0.0, 28.55,10.4);
  	a[id].setDepth(100);
	//Porta entre Sala01 e Corredor
	id = a.addLine(6.7,37.9);
  	a[id].addLinearPoint(10.2,37.9);
  	a[id].addLinearPoint(3.2,37.9);
  	a[id].setColor(XF_COLOR_RED);
  	a[id].setLine(3,XF_LINE_SOLID,0.0,	XF_CAP_ROUND,XF_JOIN_BEVEL);
	//Porta entre Sala02 e Corredor
	id = a.addLine(13.4,5.2);
  	a[id].addLinearPoint(13.4,1.7);
  	a[id].addLinearPoint(13.4,8.7);
  	a[id].setColor(XF_COLOR_RED);
  	a[id].setLine(3,XF_LINE_SOLID,0.0,	XF_CAP_ROUND,XF_JOIN_BEVEL);
	//Iniciação de variável que vai guardar o caminho a ser percorrido
	id = a.addLine(4.1,51.5);
  	a[id].setColor(XF_COLOR_BLUE);
  	a[id].setLine(3,XF_LINE_SOLID,0.0,	XF_CAP_ROUND,XF_JOIN_BEVEL);
	id2 = a.addLine(4.1,51.5);
  	a[id2].setColor(XF_COLOR_GREEN);
  	a[id2].setLine(3,XF_LINE_SOLID,0.0,	XF_CAP_ROUND,XF_JOIN_BEVEL);
		
	//Acoplamento a semaforo sProCaminho ja criado em kapekOS.cpp
	if (sProCaminho.connect_only(SPROCAMINHO)){
	  cerr<<"Erro no acoplamento!"<<endl;
	  exit(1);
	}
	
	pthread_t threadCo;
    pthread_create(&threadCo,NULL,controlePosicao,NULL);
	
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

bool testaRegiao(PONTO xyatual, PONTO xyfinal, float erro){
	
	float deltaX=0.0, deltaY=0.0, deltaL=0.0;		
	
	//Calcula a distancia do ponto atual para o ponto destino
	deltaX = xyfinal.x - xyatual.x;
	deltaY = xyfinal.y - xyatual.y;
	deltaL = hypot(deltaX,deltaY);
	
	if (deltaL > erro){			
		return (false);
	}
	return (true);
}
