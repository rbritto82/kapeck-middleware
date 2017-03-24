/***************************************************************************
 *            actionPl_pathPl.cpp
 *
 *  Wed Sep  5 10:58:28 2007
 *  Copyright  2007  User Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <math.h>
#include "Mapa.h"
#include "Planejamento.h"
#include "bBoardAtt.h"
#include "dados.h"
#include "xfig.h"
#include "bBoardRe.h"
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

using namespace std;

int main(int argc, char* argv[]){
	
	int size=0;
	PONTO xyinicial;	
	bBoardAtt<PONTO> bReferencia(BREFERENCIA);
	bBoardAtt<bool> bFim(BFIM);
	bBoardAtt<PONTO> bLocalizacao(BLOCALIZACAO);
	bBoardRe<VISAO> bVisao("10.13.99.227",BVISAO);
	VISAO visao;
	CAMINHO caminho;
	vector<Ambiente*> rota;
	semaphore sProCaminho;
	PONTO pontos[3],posRobo;
	AMBIENTE ambMarca = {0,false};
	AMBIENTE ambRobo = {0,false};
	XFig a(XF_PORTRAIT);
	
	//Variaveis usadas para plotar tempo gasto em cada execucao do laco de controle
	int z=0;
	long servert[20];
	struct timeval tvServer1,tvServer2;
    struct timezone tzServer1,tzServer2;
	long difServer, nowServer1, nowServer2;
	FILE *pFileServer;
	
	visao.status = false;
	
	//Acoplamento a semaforo sProCaminho ja criado em kapeck1.cpp
	if (sProCaminho.connect_only(SPROCAMINHO)){
	  cerr<<"Erro no acoplamento!"<<endl;
	  exit(1);
	}
	
	if(argc > 1){
		xyinicial.x = atof(argv[1]);
		xyinicial.y = atof(argv[2]);
		xyinicial.ang = atof(argv[3]);
	}
	else{
		xyinicial.x = 0.41;
		xyinicial.y = 5.15;
		xyinicial.ang = -M_PI/2;
	}
	
	//--------------------Criacao e Configuracao do mapa( Em metros)------------
	Mapa* mapa = new Mapa(0.51);
	//Sala01 
	mapa->criarAmbiente(string("SALA01"),0.0,3.79,1.34,5.6);
	//Corredor
	mapa->criarAmbiente(string("CORREDOR"),0.0,0.0,1.34,3.79);
	//Sala02
	mapa->criarAmbiente(string("SALA02"),1.34,0.0,2.855,1.04);	
	//Porta entre Sala01 e corredor
	mapa->criarPorta(0.67,3.79,0.70,0.30,HORIZONTAL);
	//Porta entre Sala02 e corredor
	mapa->criarPorta(1.34,0.52,0.70,0.30,VERTICAL);		
	//--------------------------------------------------------------------------
	
	//Recuperacao dos valores dos ambientes 
	Ambiente* s1  = &(*mapa->getAmbientes())[0];
	Ambiente* c  = &(*mapa->getAmbientes())[1];
	Ambiente* s2  = &(*mapa->getAmbientes())[2];		
	
	//Recuperacao dos valores das portas
	Porta* p1 = mapa->getPorta(0);
	Porta* p2 = mapa->getPorta(1);
	
	//Ligando sala01 e corredor a porta01
	mapa->attAmbPorta(s1,p1);
	mapa->attAmbPorta(c,p1);
	//Ligando sala02 e corredor a porta02
	mapa->attAmbPorta(s2,p2);
	mapa->attAmbPorta(c,p2);
		
	//Informa posicao atual do robo
	bLocalizacao.write(xyinicial);
	
	Planejamento* planejamento = new Planejamento();	
	
	//Gera ordem de exploracao
	rota = planejamento->planejarRota(mapa,s1);
	
	//Plota a rota a ser seguida pelo robo
	cout << "------Ordem de visita------" << endl;
	for(unsigned i=0;i<rota.size()-2;i++){
		cout << rota[i]->getNome() << endl;
	}
	cout << "---------------------------" << endl;
	
	for(unsigned i=0;i<rota.size()-3;i++){
				
		//Transforma ordem de exploracao em pontos, gerando sempre os pontos
		//necessarios para ir de um ambiente a outro
		posRobo = bLocalizacao.read();
		caminho = planejamento->planejarCaminho(i);
				
		//Testa se determinado caminho leva a um ambiente explorado ou nao
		if(caminho.p1.x==0.0 && caminho.p1.y==0.0){
			pontos[0] = caminho.p2;
			pontos[1] = caminho.p3;
			size = 2;
			
		}
		else{
			pontos[0] = caminho.p1;
			pontos[1] = caminho.p2;
			pontos[2] = caminho.p3;
			size = 3;
		}
		
		//Plota os pontos do caminho atual
		cout << "------Pontos Principais do Caminho------" << endl;
		if((caminho.p1.x != 0.0) && (caminho.p1.y != 0.0))
			cout << "("<<caminho.p1.x <<" , "<<caminho.p1.y<<")"<< endl;
		cout << "("<<caminho.p2.x <<" , "<<caminho.p2.y<<")"<< endl;
		cout << "("<<caminho.p3.x <<" , "<<caminho.p3.y<<")"<< endl;
		cout << "----------------------------------------" << endl; 
		
		//laco de execucao de trajetoria
		for(int i=0;i<size;i++){	
			//Seta momento base para captura de tempo consumido pelo laco de controle
			gettimeofday(&tvServer1,&tzServer1);
    		nowServer1=((tvServer1.tv_sec*1000000)+tvServer1.tv_usec);
			
			visao = (VISAO)bVisao.read();
			
			//Guarda tempo real do periodo de amostragem
			gettimeofday(&tvServer2,&tzServer2);
    		nowServer2=((tvServer2.tv_sec*1000000)+tvServer2.tv_usec);
			difServer = nowServer2 - nowServer1;
			servert[z] = difServer;
			z++;
			
			if (visao.status==false){				
				bReferencia.write(pontos[i]);
				cout<<"Xref= "<<pontos[i].x<<" Yref= "<<pontos[i].y<<endl;				
				//Decremento do semaforo sProCaminho		
				if (sProCaminho.decrement()){
	  				cerr<<"Erro no decremento!"<<endl;
	  				exit(1);
				}	
			}				
			else{
				ambMarca = planejamento->localizarPonto(visao.marca);
				ambRobo = planejamento->localizarPonto(bLocalizacao.read());
				if (ambRobo.amb == ambMarca.amb){				
					bReferencia.write(visao.marca);
					cout<<"X="<<visao.marca.x<<endl;					
					if (sProCaminho.decrement()){
	  					cerr<<"Erro no decremento!"<<endl;
	  					exit(1);
					}
					break;
				}
				else{
					caminho = planejamento->planejarCaminhoMarca(ambRobo.amb, ambMarca.amb);
					pontos[0] = caminho.p2;
					pontos[1] = caminho.p3;
					pontos[2] = visao.marca;
					//Plota os pontos do caminho atual
					cout << "------Caminho para Marca------" << endl;		
					cout << "("<<pontos[0].x <<" , "<<pontos[0].y<<")"<< endl;
					cout << "("<<pontos[1].x <<" , "<<pontos[1].y<<")"<< endl;
					cout << "("<<pontos[2].x <<" , "<<pontos[2].y<<")"<< endl;
					cout << "----------------------------------------" << endl; 
					for(int i=0;i<size;i++){
						bReferencia.write(pontos[i]);								
						//Decremento do semaforo sProCaminho		
						if (sProCaminho.decrement()){
	  						cerr<<"Erro no decremento!"<<endl;
	  						exit(1);
						}
					}
					break;
				}
								
			}			
		}
		if (visao.status==true) break;
	}
	bFim.write(true);
	//Salva em arquivo os tempos adquiridos
	pFileServer = fopen("server", "w");	
	for(int h=0;h<z;h++){		
		fprintf(pFileServer, "%i %lf \n", h, servert[h]/1000.0);
	}
	
	delete planejamento;
	cout<<"Saindo..."<<endl;
	return 0;
}
