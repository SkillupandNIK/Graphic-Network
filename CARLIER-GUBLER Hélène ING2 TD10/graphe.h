#ifndef GRAPHE_H
#define GRAPHE_H

#include "sommet.h"
#include "arete.h"
#include <string>
#include <vector>

class Graphe
{
public :
    //données
    std::vector<std::vector<int> > MatriceAdjacence;
    int ordre;
    std::vector<sommet> Vsommet;

    //constructeur
    Graphe();
    //destructeur
    ~Graphe();

    void RecuperationFichier();
    void AffichageInfluence();

};

#endif // GRAPHE_H
