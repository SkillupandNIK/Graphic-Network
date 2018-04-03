#ifndef GRAPHE_H
#define GRAPHE_H

#include "sommet.h"
#include "arete.h"
#include <string>
#include <vector>
#include <allegro.h>

class Graphe
{
public :
    //données
    std::vector<std::vector<int> > MatriceAdjacence;
    int m_ordre;
    std::vector<sommet> Vsommet;
    BITMAP* buffer;
    BITMAP* collision;
    BITMAP* grapheB;

    //constructeur
    Graphe();
    //destructeur
    ~Graphe();

    void RecuperationFichier();
    BITMAP* AffichageInfluence();

};

#endif // GRAPHE_H
