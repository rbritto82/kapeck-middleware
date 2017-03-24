#include "Ambiente.h"

Ambiente::Ambiente(){}

Ambiente::Ambiente(string nome, float xmin, float ymin, float xmax, float ymax){
    Ambiente();
    this->nome = nome;
    this->xmin = xmin;
    this->xmax = xmax;
	this->xcentro = (xmax+xmin)/2;
    this->ymin = ymin;
    this->ymax = ymax;
	this->ycentro = (ymax+ymin)/2; 
    this->largura = fabs(xmax - xmin);
    this->altura = fabs(ymax - ymin);
}

void Ambiente::addPorta(Porta* porta){
    portas.push_back(porta);
}

vector<Ambiente*>* Ambiente::getAmbAdjacentes(){
    Ambiente* amb;

    ambAdjacentes.clear();

    for(int i=0;i<getNumPortas();i++){
        amb = portas[i]->getAmbiente(0);
        if(amb == this){    //obtém o outro ambiente
            amb = portas[i]->getAmbiente(1);
        }
        ambAdjacentes.push_back(amb);
    }
    return &ambAdjacentes;
}
