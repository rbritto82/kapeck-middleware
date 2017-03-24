/***********************************************************************
 * Module:  Entrada.h
 * Author:  Max
 * Created: quinta-feira, 4 de maio de 2006 21:22:05
 * Purpose: Declaration of the class Entrada
 ***********************************************************************/

#ifndef Porta_h
#define Porta_h

class Ambiente;

enum Orientacao{VERTICAL, HORIZONTAL};
enum Lado{DIREITO,ESQUERDO,SUPERIOR,INFERIOR};

class Porta{
    private:
        float x;
        float y;
        float tamanho;
        float espessura;
        Orientacao orientacao;
        Ambiente* ambientes[2];
        int numAmbientes;
    protected:
    public:
        Porta(float x, float y, float tamanho, float espessura, Orientacao orientacao);
        int addAmbiente(Ambiente* ambiente);
        float getX(){return x;};
        float getY(){return y;};
        Orientacao getOrientacao(){return orientacao;};
        float getTamanho(){return tamanho;};
        int getNumAmbientes(){return numAmbientes;};
        float getEspessura(){return espessura;}
        Ambiente* getAmbiente(int i){return ambientes[i];};
};

#endif
