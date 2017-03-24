/***********************************************************************
 * Module:  Mapa.h
 * Author:  Max
 * Created: quinta-feira, 4 de maio de 2006 20:29:55
 * Purpose: Declaration of the class Mapa
 ***********************************************************************/

#if !defined(__Mapa_h)
#define __Mapa_h
#include <vector>
#include <string>
#include "Porta.h"
#include "Ambiente.h"

class Mapa{
    private:
        float diamRobo;
        vector<Ambiente> ambientes;
        vector<Porta> portas;
        vector<Porta*> portasAdj;
    public:
        Mapa(float diamRobo);
        Ambiente* criarAmbiente(string nome, float xmin, float ymin, float xmax, float ymax);
        Porta* criarPorta(float x, float y, float tamanho,float espessura, Orientacao orientacao);
        int attAmbPorta(Ambiente* ambiente, Porta* porta);
        int getNumAmbientes(){return ambientes.size();};
        int getNumPortas(){return portas.size();};
        vector<Porta*>* getPortaComum(Ambiente* amb1, Ambiente* amb2);
        Ambiente* getAmbiente(unsigned i){return &ambientes[i];};
        Porta* getPorta(unsigned i){return &portas[i];};
        vector<Ambiente>* getAmbientes(){return &ambientes;};
        vector<Porta>* getPortas(){return &portas;};
        Lado ladoPorta(Ambiente* ambiente, Porta* porta);
};

#endif
