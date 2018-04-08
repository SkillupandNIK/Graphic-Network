#include "grman/grman.h"
#include <iostream>
#include "main.h"

///using namespace grman;
int main()
{
//    WidgetButton Sauvegarder;

    int taille_screen_x = 1024;
    int taille_screen_y = 728;
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");

    BITMAP* Poubelle;
    BITMAP* Menu;
    /// Un exemple de graphe

    BITMAP* MenuB;
    BITMAP* page_Menu;
    /// Un exemple de graphe


    Poubelle = load_bitmap("poubelle.bmp", NULL);
  //  Menu = create_bitmap(taille_screen_x, taille_screen_y);
    MenuB = load_bitmap("FarmvilleBG.bmp", NULL);
    if(MenuB == NULL)
        exit(69);


    page_Menu = create_bitmap(taille_screen_x, taille_screen_y);

    clear_to_color(screen, makecol(200,19,0));

    //clear_to_color(Menu, makecol(255,255,0));
    blit(MenuB, page_Menu, 0, 0, 0, 0, 1024, 728);

    int choix_test=0;

    Poubelle = load_bitmap("poubelle.bmp", NULL);
    Menu = create_bitmap(taille_screen_x,taille_screen_y);
    //clear_to_color(screen, makecol(255,255,0));
    clear_to_color(Menu, makecol(255,255,0));
    if(Poubelle != NULL)
    {
        masked_blit(Poubelle, Menu, 0,0,taille_screen_x-90,taille_screen_y-90,80,80);
    }
    while(draw_bouton_texte(Menu,taille_screen_x/2, taille_screen_y-40, taille_screen_x/2+40, taille_screen_y-20, makecol(255,255,255),makecol(255,0,0),3, "Quitter") != 1)
    {
        Graph g;
        if(draw_bouton_image(Menu,10,taille_screen_y-90,80+10,taille_screen_y-10,makecol(255,255,255),makecol(255,0,0),3,"plus.bmp") == 1)
        {
            std::cout << "On rajoute un graphe";
        }
        else if(draw_bouton_texte(page_Menu,taille_screen_x/2-40, taille_screen_y/2-40, taille_screen_x/2+40, taille_screen_y/2-20, makecol(255,0,255),makecol(255,0,0),3, "Ferme") == 1 || choix_test == 1)
                    choix_test=GestionGraphe(g, "ChaineSauvee.txt");

        else if(draw_bouton_texte(page_Menu,taille_screen_x/2-45, taille_screen_y/2+60, taille_screen_x/2+45, taille_screen_y/2+80, makecol(255,0,255),makecol(255,0,0),3, "Ferme Mer") == 1 || choix_test == 2)
                    choix_test=GestionGraphe(g, "FermeMer.txt");

        else if(draw_bouton_texte(page_Menu,taille_screen_x/2-70, taille_screen_y/2+160, taille_screen_x/2+70, taille_screen_y/2+180, makecol(255,0,255),makecol(255,0,0),3, "Ferme Imaginaire") == 1 || choix_test == 3)
                    choix_test=GestionGraphe(g, "Chaineimaginaire.txt");


        blit(page_Menu, screen,0,0,0,0,1024,728);
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();

int GestionGraphe(Graph& g, std::string nom_graphe)
{

    int val_up;
    int reset_test = 0;

    g.make_example(nom_graphe);

    while ( !key[KEY_ESC] )
    {

        if (key[KEY_SPACE])
            g.test_remove_vertex(1);

        if (g.update(nom_graphe) == 1)
        {
            reset_test = 1;
            val_up = 1;
        }
        else if (g.update(nom_graphe) == 2)
        {
             reset_test = 1;
             val_up=2;
        }
        else if (g.update(nom_graphe) == 3)
        {
             reset_test = 1;
             val_up=3;
        }
        else if (val_up==1 && val_up==2 && val_up==3)
            val_up=0;

        if (key[KEY_I])
            g.k_connexite();

        if (key[KEY_O])
        {
            int compteur = 0;
            g.ColoriageCompoConnexe();
            for(compteur = 0; compteur < 250; compteur ++)
                g.AffichageGraphReduit(compteur);
        }
        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        g.update();
        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
    return val_up;
}

/*
SOURCE :
http://codes-sources.commentcamarche.net/source/31767-dessin-d-un-bouton-a-enfoncer-en-allegro-c
*/
int draw_bouton_texte(BITMAP *buffer, int posx, int posy, int pos2x, int pos2y, int color1, int color2, int profondeur, char* texte)
    {
    int pos_mouse=1; // Pour savoir si la souris est sur le bouton
    int color; // Pour definir la couleur du bouton
    int c_hg; // Couleur du coin haut gauche
    int c_bd; // Couleur du coin bas droite
    int texte_x; // Position du texte en x
    int texte_y; // Position du texte en y
    int retour=0; // Renvoi
    int i;

    if(profondeur >5)profondeur=5;
    if(profondeur <=0)profondeur=1;

    // Si la souris est sur le bouton on modifie la valeur de pos_mouse
    if(mouse_x>=posx && mouse_x<=pos2x && mouse_y>=posy && mouse_y<=pos2y)
        {
        pos_mouse=2;
        // Si l'utilisateur clique sur le bouton
        if(mouse_b & 1)
            {
            pos_mouse=3;
            // On renvoie un 1 si le bouton est enfonc�
            retour=1;
            }
        }
    else pos_mouse=1;

    // Changement des couleurs pour les effets de perspective et pour savoir si la souris est sur le bouton
    switch(pos_mouse)
        {
        case 1:c_hg=255;c_bd=0;
               color=color1;
               texte_x=(posx+pos2x)/2;
               texte_y=(posy+pos2y)/2;
               break;
        case 2:c_hg=255;c_bd=0;
               color=color2;
               texte_x=(posx+pos2x)/2;
               texte_y=(posy+pos2y)/2;
               break;
        case 3:c_hg=0;c_bd=255;
               color=color2;
               texte_x=((posx+pos2x)/2)+profondeur;
               texte_y=((posy+pos2y)/2)+profondeur;
               break;
        }

    // Dessin de lignes blanches et noires pour la perspective
    for(i=0;i!=profondeur;i++)
        {
        hline(buffer,posx,posy+i,pos2x-i,makecol(c_hg,c_hg,c_hg));
        vline(buffer,posx+i,posy,pos2y-i,makecol(c_hg,c_hg,c_hg));
        hline(buffer,posx+i,pos2y-i,pos2x,makecol(c_bd,c_bd,c_bd));
        vline(buffer,pos2x-i,posy+i,pos2y,makecol(c_bd,c_bd,c_bd));
        }

    // Remplissage du bouton
    rectfill(buffer,posx+profondeur,posy+profondeur,pos2x-profondeur,pos2y-profondeur,color);
    // Ecriture du texte
    textprintf_centre(buffer,font,texte_x,texte_y,makecol(255,255,255),"%s",texte);

    // Renvoi : 1 si le bouton a �t� cliqu�, 0 sinon
    return retour;
}

int draw_bouton_image(BITMAP *buffer, int posx, int posy, int pos2x, int pos2y, int color1, int color2, int profondeur, char* nom_image)
    {
    int pos_mouse=1; // Pour savoir si la souris est sur le bouton
    int color; // Pour definir la couleur du bouton
    int c_hg; // Couleur du coin haut gauche
    int c_bd; // Couleur du coin bas droite
    BITMAP* image;
    image = load_bitmap(nom_image,NULL);
    int image_x; // Position de l'image en x
    int image_y; // Position de l'image en y
    int retour=0; // Renvoi
    int i;

    if(profondeur >5)profondeur=5;
    if(profondeur <=0)profondeur=1;

    // Si la souris est sur le bouton on modifie la valeur de pos_mouse
    if(mouse_x>=posx && mouse_x<=pos2x && mouse_y>=posy && mouse_y<=pos2y)
        {
        pos_mouse=2;
        // Si l'utilisateur clique sur le bouton
        if(mouse_b & 1)
            {
            pos_mouse=3;
            // On renvoie un 1 si le bouton est enfonc�
            retour=1;
            }
        }
    else pos_mouse=1;

    // Changement des couleurs pour les effets de perspective et pour savoir si la souris est sur le bouton
    switch(pos_mouse)
        {
        case 1:c_hg=255;c_bd=0;
               color=color1;
               image_x=(posx+pos2x)/2;
               image_y=(posy+pos2y)/2;
               break;
        case 2:c_hg=255;c_bd=0;
               color=color2;
               image_x=(posx+pos2x)/2;
               image_y=(posy+pos2y)/2;
               break;
        case 3:c_hg=0;c_bd=255;
               color=color2;
               image_x=((posx+pos2x)/2)+profondeur;
               image_y=((posy+pos2y)/2)+profondeur;
               break;
        }

    // Dessin de lignes blanches et noires pour la perspective
    for(i=0;i!=profondeur;i++)
        {
        hline(buffer,posx,posy+i,pos2x-i,makecol(c_hg,c_hg,c_hg));
        vline(buffer,posx+i,posy,pos2y-i,makecol(c_hg,c_hg,c_hg));
        hline(buffer,posx+i,pos2y-i,pos2x,makecol(c_bd,c_bd,c_bd));
        vline(buffer,pos2x-i,posy+i,pos2y,makecol(c_bd,c_bd,c_bd));
        }

    // Remplissage du bouton
    rectfill(buffer,posx+profondeur,posy+profondeur,pos2x-profondeur,pos2y-profondeur,color);
    // Blit de l'image
    masked_blit(image, buffer,0,0,posx, posy, pos2x-posx, pos2y-posy);
    //textprintf_centre(buffer,font,texte_x,texte_y,makecol(255,255,255),"%s",texte);

    // Renvoi : 1 si le bouton a �t� cliqu�, 0 sinon
    return retour;
}

