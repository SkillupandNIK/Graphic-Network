#ifndef ARETE_H
#define ARETE_H

#include "sommet.h"
#include <string>


class arete
{
public:
    //Donnée
    sommet* Extremites[2];
    std::string m_nom;

    //constructeur
    arete(std::string nom = "");

    //destructeur
    ~arete();
};

#endif // ARETE_H
