#include <iostream>
#include <ctime>
#include "graphe.h"

using namespace std;

void lancerAllegro()
{
    srand(time(NULL));
    //initialisation générale
    allegro_init();
    //pour utiliser le clavier
    install_keyboard();
    //pour utiliser la souris
    install_mouse();


    set_color_depth(desktop_color_depth());//on fixe le nombre de bit sur lesquels sont codées les couleurs

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0) != 0)
    {
        allegro_message("probleme mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    show_mouse(screen);
}

int main()
{
    std::cout << "coucou1" << std::endl;
    srand(time(NULL)); //on initialise le temps
    std::cout << "coucou2" << std::endl;
    BITMAP* buffer;
    std::cout << "coucou3" << std::endl;
    Graphe conseil;
    std::cout << "coucou4" << std::endl;
    conseil.RecuperationFichier();
    std::cout << "coucou5" << std::endl;
    buffer = conseil.AffichageInfluence();
    std::cout << "coucou6" << std::endl;
    draw_sprite(screen, buffer, 0, 0);
    std::cout << "coucou7" << std::endl;
}
END_OF_MAIN();
