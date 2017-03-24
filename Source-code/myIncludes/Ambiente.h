/***********************************************************************
 * Module:  Ambiente.h
 * Author:  Max
 * Created: quinta-feira, 4 de maio de 2006 20:29:40
 * Purpose: Declaration of the class Ambiente
 ***********************************************************************/

#ifndef Ambiente_h
#define Ambiente_h

#include <vector>
#include <string>
#include "Porta.h"
#include <math.h>

using namespace std;

class Ambiente{
private:
    string nome;
    float largura, altura;
    float xmin,xmax,ymin,ymax;
	float xcentro,ycentro;
    vector<float> linhasHorizontais;
    vector<float> linhasVerticais;
    vector<Porta*> portas;
    bool visitado;
    vector<Ambiente*> ambAdjacentes;

protected:
public:
    Ambiente();
    Ambiente(string nome, float xmin, float ymin, float xmax, float ymax);
    void addPorta(Porta* porta);
    inline int getNumPortas(){return portas.size();};
    inline string getNome(){return nome;}
    inline float getAltura(){return altura;}
    inline float getLargura(){return largura;}
    inline float getXMin(){return xmin;}
    inline float getXMax(){return xmax;}
	inline float getXCentro(){return xcentro;}
    inline float getYMin(){return ymin;}
    inline float getYMax(){return ymax;}
	inline float getYCentro(){return ycentro;}
    inline bool isVisitado(){return visitado;};
    inline void setVisitado(bool vis){visitado = vis;};
    inline vector<Porta*>* getPortas(){return &portas;};
    vector<Ambiente*>* getAmbAdjacentes();
};

#endif
