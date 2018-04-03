#ifndef SOMMET_H
#define SOMMET_H

#include <string>
#include <vector>
#include <allegro.h>

class sommet
{
 private:
    //Donnée
    std::string m_nom;
    int m_numero;
    std::vector<int> m_coordonnee;
    BITMAP* m_image;

 public:
    //constructeur
    sommet(std::string nom = "", int numero = 0, std::vector<int> coordonnee = {0,0});
    //destructeur
    ~sommet();

    //getters
    std::string getNom();
    std::vector<int> getCoordonnee();
    int getNum();
    BITMAP* getImage();

    //setters
    void setNom(std::string nom);
    void setCoordonnee(std::vector<int> coordonnee);
    void setNum(int numero);



};

#endif // SOMMET_H
