//---------------------------------------------------------------------------

#ifndef PlanejamentoH
#define PlanejamentoH

#include "Mapa.h"
#include "dados.h"
#include <iostream>
#include <vector>
#define pi 3.14

class Planejamento{
    private:
        Mapa* mapa;
        vector<Ambiente*> ordem;
        void buscaProfundidade(Ambiente* ambiente);
		CAMINHO caminho;
		void irParaCentro(Ambiente* ambiente);
        void irParaAmbiente(Ambiente* amb1, Ambiente* amb2);
		
    public:
        Planejamento();
        vector<Ambiente*> planejarRota(Mapa* argmapa, Ambiente* ambInicial);
		struct CAMINHO planejarCaminho(unsigned i);
		AMBIENTE localizarPonto(PONTO xy);
		struct CAMINHO planejarCaminhoMarca(int ambRobo, int ambMarca);
};

//---------------------------------------------------------------------------
#endif
