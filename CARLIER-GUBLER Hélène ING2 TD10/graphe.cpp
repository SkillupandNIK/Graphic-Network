#include "graphe.h"
#include <fstream>
#include <iostream>


Graphe::Graphe()
{
    m_ordre = 0;
}

Graphe::~Graphe()
{

}

void Graphe::RecuperationFichier()
{
    std::cout << "coucou1.1" << std::endl;
    int f_matrice;
    std::string f_nom;
    int numero;
    int x,y;
    std::cout << "coucou1.2" << std::endl;
    if(!buffer)
        buffer = create_bitmap(SCREEN_W,SCREEN_H);
    std::cout << "coucou je suis la 1" << std::endl;
    if(!collision)
    {
        std::cout << "Collision est nul !" << std::endl;
        collision = create_bitmap(SCREEN_W,SCREEN_H);
        std::cout << "je vais bientot sortir de la boucle ! " << std::endl;
    }

    std::cout << "coucou je suis la 2" << std::endl;
    if(!grapheB)
        grapheB = create_bitmap(SCREEN_W,SCREEN_H);
    std::cout << "coucou je suis la 3" << std::endl;
    std::cout << "coucou1.3" << std::endl;
    if (!buffer || !collision || !grapheB)
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Erreur ! Impossible de lire les fichiers !");
    }
    std::cout << "coucou1.4" << std::endl;
    std::ifstream Admin("Chaine1.txt", std::ios::in);
    std::cout << "coucou1.5" << std::endl;
    if(Admin == NULL)
        std::cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;

    std::cout << "coucou1.5" << std::endl;
    Admin >> m_ordre;
    std::vector < std::vector < int > > mat (m_ordre);
    MatriceAdjacence = mat;
    std::cout << "coucou1.6" << std::endl;
    for(int i = 0 ; i<m_ordre ; i++)
    {

        for (int j = 0 ; j<m_ordre; j++)
        {
            Admin >> f_matrice;
            MatriceAdjacence[i].push_back(f_matrice);
        }
    }
    std::cout << "coucou1.7" << std::endl;
    for(int k = 0; k<m_ordre; k++)
    {
        sommet a;
        Admin >> numero;
        Admin >> f_nom;
        Admin >> x;
        Admin >> y;
        a = sommet(f_nom, numero, {x,y});
        Vsommet.push_back(a);
    }
    std::cout << "coucou1.8" << std::endl;
}

BITMAP* Graphe::AffichageInfluence()
{
    std::cout << "ORDRE : " << m_ordre << std::endl << std::endl;
    std::cout << "MATRICE ADJACENCE" << std::endl;
    for(int i = 0 ; i<m_ordre; i++)
    {
        for(int j = 0 ; j<m_ordre; j++)
            std::cout << MatriceAdjacence[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;

    ///AFFICHAGE CONSOLE DES DONNEES
    for(int i =0 ; i<m_ordre; i++)
    {
        std::cout << "NOM : " << Vsommet[i].getNom()<< std::endl;
        std::cout << "NUMERO : " << Vsommet[i].getNum()<< std::endl;
        std::cout << "COORDONNEE : x = " << (Vsommet[i].getCoordonnee())[0]<< " y = " << (Vsommet[i].getCoordonnee())[1] << std::endl;

    }


    ///ON MET LES IMAGES DES SOMMETS
    for(int i = 0 ; i <m_ordre; i++)
    {
        std::vector<int> coord = Vsommet[i].getCoordonnee();
        blit(Vsommet[i].getImage(),grapheB, 0, 0, coord[0], coord[1], SCREEN_W, SCREEN_H);
    }

    ///ON MET LES ARCS SUR LES SOMMETS
    for(int i = 0; i <m_ordre; i++)
    {
        for(int j = 0; j<m_ordre;j++)
        {
            if(MatriceAdjacence[i][j] != 0)
            {
                std::vector<int> CoordS1 = Vsommet[i].getCoordonnee();
                std::vector<int> CoordS2 = Vsommet[j].getCoordonnee();
                line(grapheB, CoordS1[0], CoordS1[1], CoordS2[0], CoordS2[1], makecol(0,0,0));
            }
        }
    }
    blit(grapheB, buffer, 0,0,0,0,SCREEN_W, SCREEN_H);

    return buffer;
}
