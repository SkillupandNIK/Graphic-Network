#include "sommet.h"
#include <fstream>
#include <iostream>
#include <limits>
using namespace std;

sommet::sommet()
{
}

sommet::sommet(std::string nom , int numero, std::vector<int> coordonnee)
{
    m_nom = nom;
    m_numero = numero;
    m_coordonnee = coordonnee;

    std::string nomImage = nom + ".bmp";

    m_image = load_bitmap(nomImage.c_str() ,NULL);
    if ( !m_image)
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Erreur ! Impossible de lire les fichiers !");
    }

}

sommet::~sommet()
{

}
///GETTERS

std::string sommet::getNom()
{
    return m_nom;
}

std::vector<int> sommet::getCoordonnee()
{
    return m_coordonnee;
}

int sommet::getNum()
{
    return m_numero;
}

BITMAP* sommet::getImage()
{
    return m_image;
}

///SETTERS

void sommet::setNom(std::string nom)
{
    m_nom = nom;
}

void sommet::setCoordonnee(std::vector<int> coordonnee)
{
    m_coordonnee = coordonnee;
}

void sommet::setNum(int numero)
{
    m_numero = numero;
}


