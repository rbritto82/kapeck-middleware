#include "Mapa.h"
#include <math.h>

using namespace std;

Mapa::Mapa(float diamRobo){
    this->diamRobo = diamRobo;
}

Ambiente* Mapa::criarAmbiente(string nome, float xmin, float ymin, float xmax, float ymax){
    Ambiente* amb = new Ambiente(nome, xmin + diamRobo/2.0, ymin + diamRobo/2.0, xmax - diamRobo/2.0, ymax - diamRobo/2.0);
    ambientes.push_back(*amb);
    delete amb;
    return &(ambientes.back());
}

Porta* Mapa::criarPorta(float x, float y, float tamanho, float espessura, Orientacao orientacao){
    Porta* porta = new Porta(x,y,tamanho - diamRobo,espessura + diamRobo,orientacao);
    portas.push_back(*porta);
    delete porta;
    return &portas.back();
}

int Mapa::attAmbPorta(Ambiente* ambiente, Porta* porta){
    int okPorta = porta->addAmbiente(ambiente);
    ambiente->addPorta(porta);
    return okPorta;
}

vector<Porta*>* Mapa::getPortaComum(Ambiente* amb1, Ambiente* amb2){
    Porta* porta;
    Ambiente* ambp1;
    Ambiente* ambp2;
    portasAdj.clear();
    for(unsigned i=0;i<portas.size();i++){
        porta = &portas[i];
        ambp1 = porta->getAmbiente(0);
        ambp2 = porta->getAmbiente(1);
        if((amb1 == ambp1 && amb2 == ambp2) || (amb2 == ambp1 && amb1 == ambp2)){
            portasAdj.push_back(porta);
        }
    }
    return &portasAdj;
}

Lado Mapa::ladoPorta(Ambiente* ambiente, Porta* porta){
    if(porta->getOrientacao() == HORIZONTAL){
        float distInf = fabs(porta->getY() - ambiente->getYMin());
        float distSup = fabs(porta->getY() - ambiente->getYMax());

        if(distInf < distSup)
            return INFERIOR;
        else
            return SUPERIOR;
    }
    else{
        float distEsq = fabs(porta->getX() - ambiente->getXMin());
        float distDir = fabs(porta->getX() - ambiente->getXMax());

        if(distEsq < distDir)
            return ESQUERDO;
        else
            return DIREITO;
    }
}
