#include "Planejamento.h"

using namespace std;

Planejamento::Planejamento(){}

//-------------------Planejador de Tarefa---------------------------------------
vector<Ambiente*> Planejamento::planejarRota(Mapa* argmapa, Ambiente* ambInicial){
	mapa = argmapa;
    vector<Ambiente>* ambientes = mapa->getAmbientes();
    for(int i=0;i<mapa->getNumAmbientes();i++){
        (*ambientes)[i].setVisitado(false);
    }

    buscaProfundidade(ambInicial);
	
	for(int i=0;i<mapa->getNumAmbientes();i++){
        (*ambientes)[i].setVisitado(false);
    }
	
    return ordem;
}

void Planejamento::buscaProfundidade(Ambiente* ambiente){
    Ambiente* amb;
    vector<Ambiente*> adjacentes;
    adjacentes.clear();
    ambiente->setVisitado(true);
    ordem.push_back(ambiente);
    vector<Porta*>* portas = ambiente->getPortas();

    //obt� os ambientes adjacentes ao ambiente atual
    for(unsigned i=0;i<portas->size();i++){
        amb = (*portas)[i]->getAmbiente(0);
        if(amb == ambiente){    //obt� o outro ambiente
            amb = (*portas)[i]->getAmbiente(1);
        }
        adjacentes.push_back(amb);
    }

    for(unsigned i=0;i<adjacentes.size();i++){
        if(!adjacentes[i]->isVisitado()){
            buscaProfundidade(adjacentes[i]);
            ordem.push_back(ambiente);
        }
    }
}

//----------------Planejamento de Caminho---------------------------------
struct CAMINHO Planejamento::planejarCaminho(unsigned j){
    
    Ambiente* ambiente;
    Ambiente* ambPosterior;   

	caminho.p1.x = 0.0;
	caminho.p1.y = 0.0;
	caminho.p2.x = 0.0;
	caminho.p2.y = 0.0;
	caminho.p3.x = 0.0;
	caminho.p3.y = 0.0;
    ambiente = ordem[j];
	ambPosterior = ordem[j+1];
	
    if(!ambiente->isVisitado()){
       ambiente->setVisitado(true);
       irParaCentro(ambiente);
    }
	irParaAmbiente(ambiente,ambPosterior);

	return caminho;
}

void Planejamento::irParaCentro(Ambiente* ambiente){
	float xcentro = ambiente->getXCentro();
	float ycentro = ambiente->getYCentro();
	PONTO xyatual = {xcentro,ycentro,pi/2};
	caminho.p1 = xyatual;
}

void Planejamento::irParaAmbiente(Ambiente* amb1, Ambiente* amb2){
    PONTO antesSair, depoisSair;
    vector<Porta*>* portas = mapa->getPortaComum(amb1,amb2);
    
        Porta* porta = (*portas)[0];
        Lado ladoPortaAmb1 = mapa->ladoPorta(amb1,porta);
        //sair do ambiente anterior e ir para o novo ambiente
        switch(ladoPortaAmb1){
            case SUPERIOR:
                antesSair.x  = porta->getX();
                if (porta->getY()>0)
					antesSair.y  = porta->getY() - porta->getEspessura()/2;
				else
					antesSair.y  = porta->getY() + porta->getEspessura()/2;
					
				antesSair.ang = pi/2;
                depoisSair.x = porta->getX();
				if (porta->getY()>0) 
                	depoisSair.y = porta->getY() + porta->getEspessura()/2;
				else
					depoisSair.y = porta->getY() - porta->getEspessura()/2;
				depoisSair.ang = pi/4;
                break;
            case INFERIOR:
                antesSair.x  = porta->getX();
				if (porta->getY()>0)
                	antesSair.y  = porta->getY() + porta->getEspessura()/2;
				else
					antesSair.y  = porta->getY() - porta->getEspessura()/2;
				antesSair.ang = 3*pi/2;
                depoisSair.x = porta->getX();
				if (porta->getY()>0)
                	depoisSair.y = porta->getY() - porta->getEspessura()/2;
				else 
					depoisSair.y = porta->getY() + porta->getEspessura()/2;
				depoisSair.ang = 7*pi/4;
                break;
            case ESQUERDO:
                if (porta->getX()>0)
					antesSair.x  = porta->getX() + porta->getEspessura()/2;
				else
					antesSair.x  = porta->getX() - porta->getEspessura()/2;
                antesSair.y  = porta->getY();
				antesSair.ang = pi;
				if (porta->getX()>0)
                	depoisSair.x = porta->getX() - porta->getEspessura()/2;
				else
					depoisSair.x = porta->getX() + porta->getEspessura()/2;
                depoisSair.y = porta->getY();
				depoisSair.ang = 3*pi/4;
                break;
            case DIREITO:
				if (porta->getX()>0)
                	antesSair.x  = porta->getX() - porta->getEspessura()/2;
				else
					antesSair.x  = porta->getX() + porta->getEspessura()/2;
                antesSair.y  = porta->getY();
				depoisSair.ang = 0;
				if (porta->getX()>0)
                	depoisSair.x = porta->getX() + porta->getEspessura()/2;
				else
					depoisSair.x = porta->getX() - porta->getEspessura()/2;
                depoisSair.y = porta->getY();
				depoisSair.ang = pi/4;
                break;
        }
       
		caminho.p2 = antesSair;
		caminho.p3 = depoisSair;
    
}

AMBIENTE Planejamento::localizarPonto(PONTO ponto){
	
	vector<Ambiente>* ambientes = mapa->getAmbientes();
    float xmin=0, ymin=0, xmax=0, ymax=0;
	AMBIENTE ambPonto = {0,false};
	
	for(int i=0;i<mapa->getNumAmbientes();i++){
        xmax = (*ambientes)[i].getXMax();
		ymax = (*ambientes)[i].getYMax();
		xmin = (*ambientes)[i].getXMin();
		ymin = (*ambientes)[i].getYMin();
				
		if ((xmin<ponto.x)&&(ponto.x<xmax)&&(ymin<ponto.y)&&(ponto.y<ymax)){
			ambPonto.amb = i;
			ambPonto.achou = true;
			break;
		}
    }
	
	return ambPonto;
}

struct CAMINHO Planejamento::planejarCaminhoMarca(int ambRobo, int ambMarca){
    
    Ambiente* ambiente = mapa->getAmbiente(ambRobo);
    Ambiente* ambPosterior = mapa->getAmbiente(ambMarca);   
		
	caminho.p1.x = 0.0;
	caminho.p1.y = 0.0;
	caminho.p2.x = 0.0;
	caminho.p2.y = 0.0;
	caminho.p3.x = 0.0;
	caminho.p3.y = 0.0;	
		
    irParaAmbiente(ambiente,ambPosterior);
	
	return caminho;
}
