#include "graphe.h"
#include <fstream>
#include <iostream>


Graphe::Graphe()
{
    ordre = 0;
}

Graphe::~Graphe()
{

}

void Graphe::RecuperationFichier()
{
    int f_matrice;
    std::string f_nom;

    std::ifstream Admin("Information.txt", std::ios::in);


    if(Admin == NULL)
        std::cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;


    Admin >> ordre;
    std::vector < std::vector < int > > mat (ordre);
    MatriceAdjacence = mat;
    for(int i = 0 ; i<ordre ; i++)
    {

        for (int j = 0 ; j<ordre; j++)
        {
            Admin >> f_matrice;
            MatriceAdjacence[i].push_back(f_matrice);
        }
    }
    for(int k = 0; k<6; k++)
    {
        sommet a;
        Admin >> f_nom;
        a = sommet(f_nom);
        Vsommet.push_back(a);
    }
}

void Graphe::AffichageInfluence()
{
    std::cout << "ORDRE : " << ordre << std::endl << std::endl;
    std::cout << "MATRICE ADJACENCE" << std::endl;
    for(int i = 0 ; i<6; i++)
    {
        for(int j = 0 ; j<6; j++)
            std::cout << MatriceAdjacence[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for(int i =0 ; i<6; i++)
        std::cout << "NOM : " << Vsommet[i].m_nom << std::endl;

    std::cout << std::endl << std::endl;

    for(int i = 0 ; i < ordre; i++)
    {
        for(int j = 0 ; j < ordre ; j++)
        {
            if(MatriceAdjacence[i][j] == 1)
            {
                std::cout << Vsommet[i].m_nom << " influence " << Vsommet[j].m_nom << std::endl;
            }
        }
    }

}
