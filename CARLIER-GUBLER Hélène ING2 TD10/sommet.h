#ifndef SOMMET_H
#define SOMMET_H

#include <string>

class sommet
{
public:
    //Donn�e
    std::string m_nom;

    //constructeur
    sommet(std::string nom = "");

    //destructeur
    ~sommet();

};

#endif // SOMMET_H
