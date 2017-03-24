#include "Porta.h"

Porta::Porta(float x, float y, float tamanho, float espessura, Orientacao orientacao){
    this->x = x;
    this->y = y;
    this->espessura = espessura;
    this->orientacao = orientacao;
    this->tamanho = tamanho;
    numAmbientes = 0;
}

int Porta::addAmbiente(Ambiente* ambiente){
    if(numAmbientes < 2){
        ambientes[numAmbientes] = ambiente;
        numAmbientes++;
        return 0;
    }
    else{
        return -1;
    }
}

