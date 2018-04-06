#include "graph.h"
#include <fstream>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{

    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    m_top_box.add_child(m_supp);
    m_supp.set_dim(10, 10);
    m_supp.set_bg_color(BLANC);
    m_supp.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_supp.add_child(m_image);
    m_image.set_pic_name("croix10.bmp");

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );


}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_example(std::string nom_fichier)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
//    add_interfaced_vertex(0, 47.0, 200, 100, "clown1.jpg");
//    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
//    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
//    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
//    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
//    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 2);
//    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 2);
//    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);
    //std::string yoyo = "Homme.bmp";

    std::ifstream Admin(nom_fichier.c_str());
    int ordre;
    int numero, N, x, y;
    std::string nom_Image;
    Admin >> ordre;
    m_ordre = ordre;

    for(int i = 0 ; i < ordre; i++)
    {
        Admin >> numero;
        Admin >> N;
        Admin >> x;
        Admin >> y;
        Admin >> nom_Image;
        add_interfaced_vertex(numero, N, x, y, nom_Image.c_str());
    }

    int nbEdge, numeroEdge, S1, S2;
    float poids;
    Admin >> nbEdge;
    for(int i = 0; i<nbEdge; i++)
    {
        Admin >> numeroEdge;
        Admin >> S1;
        Admin >> S2;
        Admin >> poids;
        add_interfaced_edge(numeroEdge, S1, S2, poids);
    }


    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
//    add_interfaced_edge(0, 1, 2, 50.0);
//    add_interfaced_edge(1, 0, 1, 50.0);
//    add_interfaced_edge(2, 1, 3, 75.0);
//    add_interfaced_edge(3, 4, 1, 25.0);
//    add_interfaced_edge(4, 6, 3, 25.0);
//    add_interfaced_edge(5, 7, 3, 25.0);
//    add_interfaced_edge(6, 3, 4, 0.0);
//    add_interfaced_edge(7, 2, 0, 100.0);
//    add_interfaced_edge(8, 5, 2, 20.0);
//    add_interfaced_edge(9, 3, 7, 80.0);

//   add_interfaced_edge(0, 0, 1, 2.0);
//   add_interfaced_edge(1, 3, 1, 2.0);
//    add_interfaced_edge(2, 1, 5, 2.0);
    //add_interfaced_edge(3, 2, 5, 10);

    //add_interfaced_edge(4, 0, 4, 20);
    //add_interfaced_edge(5, 1, 4, 20);
    //add_interfaced_edge(6, 2, 4, 20);
    //add_interfaced_edge(7, 3, 4, 20);

    //supprimer_graphe();

//    test_remove_edge(0);
//    test_remove_edge(1);
//    test_remove_edge(2);
//    test_remove_edge(3);


}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
    m_vertices[idx].m_N = value;
    m_vertices[idx].m_x = x;
    m_vertices[idx].m_y = y;
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    /// OOOPS ! Prendre en compte l'arc ajouté dans la topologie du graphe !
    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}


void Graph::supprimer_graphe()
{
    ///réinitialisation et suppression de l'interface
     m_interface = nullptr;
     m_interface.reset();
     std::cout << "Interface de " << "entrer_nom" << " supprimée." << std::endl;

     for (unsigned int i = 0; i < m_vertices.size(); i++)
     {
         m_vertices[i].supprimer_vertex();
     }
     for (unsigned int i = 0; i < m_edges.size(); i++)
     {
         test_remove_edge(i);
     }

     //remove le fichier
}

void Vertex::supprimer_vertex(bool parametre)
{
    m_interface = nullptr;
    m_interface.reset();
    std::cout << "Interface de " << "name_vertex" << " supprimée." << std::endl;

    if (parametre)
    {
        //supprimer dans la mémoire vive !!
    }

}


/// eidx index of edge to remove
void Graph::test_remove_edge(int eidx)
{
    /// référence vers le Edge à enlever
    /// on parcourt la map jusqu'à l'endroit de l'indice rentré en paramètre de la fonction
    Edge &remed=m_edges.at(eidx);

    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

    /// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

    /// test : on a bien des éléments interfacés
    if (m_interface && remed.m_interface)
    {
        /// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :
        // EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        // m_interface->m_main_box.add_child(ei->m_top_edge); */
        // m_edges[idx] = Edge(weight, ei); */
        /// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr
        /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

    /// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from !
    /// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

    /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
    /// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface
    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );

    /// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

    }

void Graph::test_remove_vertex(int vidx)
{
    Vertex &remver = m_vertices.at(vidx);
    std::cout << "On enleve le sommet " << vidx << std::endl;

    for (unsigned int i = 0; i < remver.m_in.size(); i++)
        test_remove_edge(remver.m_in[i]);

    for (unsigned int i = 0; i < remver.m_out.size(); i++)
        test_remove_edge(remver.m_out[i]);

    if (m_interface && remver.m_interface)
        m_interface->m_main_box.remove_child (remver.m_interface->m_top_box);

    //m_vertices.erase(vidx);
}


//inspiré du livret de polycopié du cours de théorie des graphes 2017-2018
std::vector<bool> Graph::uneComposanteFortementConnexe(int s)
{
    //std::cout << "On rentre dans 'Une composante Fortement Connexe'" << std::endl;

    ///variables locales
    std::vector<bool> compo1, compo2; //composantes connexes directes partant de s et indirectes arrivant à s
    std::vector<bool> compo; // composantes fortement connexes : compo1 inter compo2 à retourner
    std::vector<bool> marques1, marques2;// tableau de marquage
    int x; //numero de sommets intermédiaires des composantes connexes
    bool ajoute1 = true,ajoute2 = true; ; //booléen indiquant si une nouvelle composante connexe est ajouté


    ///On alloue et initialise les tableau compo1, compo2, compo et marques
    for(int i = 0; i< m_ordre; i++ )
    {
        compo1.push_back(false);
        compo2.push_back(false);
        compo.push_back(false);
        marques1.push_back(false);
        marques2.push_back(false);
    }
    //std::cout << "Compo s = " << s << std::endl;

    ///On rend le sommet connexe
    compo1[s] = true;
    compo2[s] = true;

    ///Recherche des composantes connexes partant de s à ajouter dans compo1

    ///COMPO1
    while(ajoute1==true)
    {
        ajoute1 = false; //à chaque tour, recherche d'une nouvelle composante connexe à ajouter
        ///Pour tous les sommets x non marqués et connectés en partant de s
        ///Marquer chaque sommet x et connecter les sommets non marqués y adjacents à x
        for(x=0; x< m_ordre; x++)
        {
            if(marques1[x]==false && compo1[x]==true)
            {
                marques1[x] = 1;
                //int k = 0;
                for(const auto& elem: m_edges)
                {

                    if(elem.second.m_from == x && marques1[elem.second.m_to] == false)
                    {
                        compo1[elem.second.m_to] = true;
                        ajoute1 = true; //nouvelle composante connexe ajoutée
                    }
                //k++;
                }
            }
        }
    }
//    for(int i =0 ; i< m_ordre; i++)
//    {
//        marques[x] = false;
//        compo2.push_back(false);
//    }
//    compo2[s] = true;

//    std::cout << "compo 2 : " ;
//    for(int i = 0; i < m_ordre; i++)
//        std::cout << compo2[i] << " ";
//    std::cout << std::endl;
//
//    std::cout << "on commence la boucle pour compo 2" << std::endl;
    //int m = 0;


    ///COMPO2
    while(ajoute2 == true)
    {
        //std::cout << "On entre dans la boucle while" << std::endl << std::endl;
        ajoute2 = false;

        for(x= 0; x<m_ordre; x++)
        {
        ///On recherche des composantes connexes arrivant à s à ajouter dans compo2
            if(marques2[x]==false && compo2[x]==true)
            {
                //std::cout << "On rentre dans if(marques[x]==false && compo2[x]) " << std::endl;
                marques2[x] = true;
                //std::cout << "On met marques[x] à true : marques2["<< x<<"] = " << marques2[x] << std::endl;
                    for(const auto& elem : m_edges)
                    {   //std::cout << "CONST AUTO ELEM NOUVEAU NUMERO" << std::endl;
                        //std::cout << "m = " << m <<std::endl;
                        //std::cout << " Sommet 1 = " << elem.second.m_from << std::endl;
                        //std::cout << " Sommet 2 = " << elem.second.m_to << std::endl;
                        if(elem.second.m_to == x && marques2[elem.second.m_from] == false)
                        {
                            //std::cout << "On rentre dans le if(elem.second.m_to == x && marques[elem.second.m_from] == false) " << std::endl;
    //                        std::cout << " Sommet 1 = " << elem.second.m_from << std::endl;
    //                        std::cout << " Sommet 2 = " << elem.second.m_to << std::endl;
                            compo2[elem.second.m_from] = true;
                            ajoute2 = true;
                        }
                        //std::cout << "COMPO 2 : ";
//                        for(int l = 0; l < m_ordre; l++)
//                        {
//                            std::cout << compo2[x] << " " ;
//                        }
//                        std::cout << std::endl;
                       // m++;
                    }

                }
            }
        }

//    std::cout << "On sort des boucles whiles" << std::endl;
//
//    for(int i=0; i<m_ordre; i++)
//        std::cout<<"compo1[i]="<<compo1[i]<<" ";
//
//    std::cout<<std::endl;
//    for(int i=0; i<m_ordre; i++)
//        std::cout<<"compo2[i]="<<compo2[i]<<" ";
//
//
    //std::cout<<"Voici une compo fortement connexe du graphe:"<<std::endl;
    for(int i=0; i<m_ordre; i++)
    {
        compo[i]=compo1[i] & compo2[i];
        //std::cout<<"sommet"<<i<<"="<<compo[i]<<std::endl;

    }

//    ///Composante fortement connexe compo = intersection de compo1 et compo2
//    std::cout << "Compo : ";
//    for(x= 0; x<m_ordre; x++)
//    {
//        compo[x] = compo1[x] & compo2[x];
//        std::cout << compo[x] << " ";
//    }
//    std::cout << std::endl;

    //std::cout << "On sort de 'Une composante Fortement Connexe'" << std::endl;
    ///On retourne la composante fortement connexe compo
    return compo;
}

std::vector<std::vector<bool> > Graph::toutesLesComposantesFortementConnexes()
{
    ///Variables locales
    //std::cout << "Graph 1 " << std::endl;
    std::vector<std::vector<bool> >  tabc (m_ordre); //tableau des composantes fortement connexe à retourner
    std::vector<bool> marques; //tableau qui indique si un sommet est marqué ou pas
    //std::cout << "Graph 2 " << std::endl;
    ///Allocation et initialisation des tableaux
    for(int i = 0; i < m_ordre; i++)
    {
        marques.push_back(false);
        for(int j = 0; j < m_ordre; j++)
            tabc[i].push_back(false);

    }
    //std::cout << "Graph 3 " << std::endl;

    ///Pour tous les sommmets non marqués
    ///Recherche de la composante fortement connexe de x
    ///Marquer chaque sommet x et marquer les sommets y connectés à x et non marqués
    for(int x =0; x< m_ordre; x++)
    {
        if(marques[x] == false)
        {
            //std::cout << "Graph 3.1 " << std::endl;
            //std::cout << "TOUTE CONNEXE X =  " << x << std::endl;
            std::vector<bool> LacompoConnexe = uneComposanteFortementConnexe(x);
            tabc[x] = LacompoConnexe;
            //std::cout << "Graph 3.2 " << std::endl;
            marques[x] = true;
            for(int y ; y<m_ordre; y++)
            {
                tabc[x][y] = LacompoConnexe[y];
                if(tabc[x][y]!=0 && marques[y]==false)
                    marques[y] = true;
            }
        }
    }
    //std::cout << "Graph 4 " << std::endl;

    return tabc;
}


void Graph::ColoriageCompoConnexe()
{
    std::vector<std::vector<bool> > CompoConnexe = toutesLesComposantesFortementConnexes();
    ///On met les arcs de couleur en fonction des composantes fortement connexes
    for(int x = 0; x < m_ordre; x++)
    {
        for(int y = 0; y< m_ordre; y++)
        {
            int couleur = COULEURALEATOIRECLAIR;

            for(const auto& elem : m_edges)
            {
                if(CompoConnexe[x][elem.second.m_from] == true && CompoConnexe[x][elem.second.m_to]==true)
                    m_edges.at(elem.first).m_interface->m_top_edge.set_color_fleche(couleur);
            }
        }
    }

    ///On créer une boite de texte qui va afficher le nombre de composantes fortement connexe à l'écran
    m_interface->m_top_box.add_child(m_interface->m_compoConnexe_text);
    //m_interface->m_compoConnexe_box(m_interface->m_tool_box);

    bool idem;
    std::string Connexe = " ";
    std::vector <std::vector<bool> > tmp;

    tmp.push_back(CompoConnexe[0]);

    ///On compte le nombre de composantes fortements connexes
    for(int i = 1; i < m_ordre; i++)
    {
        idem = false;
        for(unsigned int j = 0; j< tmp.size(); j++)
        {
            if(CompoConnexe[i] == tmp[j])
                idem = true;
        }
        if(idem == false)
            tmp.push_back(CompoConnexe[i]);
    }
    //std::cout << "Il y a " << tmp.size() << " composante(s) connexe(s) : " << std::endl;

    ///Affichages des sommets des composantes connexes
    for(unsigned int i = 0; i < tmp.size(); i++)
    {
        for(unsigned int j = 0; j< tmp[i].size(); j++)
        {
            if(tmp[i][j] != 0)
            {
                //std::cout << j << " " ;
                Connexe = Connexe + std::to_string(j) + " ";
            }

        }
        Connexe = Connexe + ",";
        //std::cout << std::endl;
    }

    Connexe = "Il y a " + std::to_string(tmp.size()) + " composante(s) fortement connexe(s) : " + Connexe;
    //std::cout << Connexe;
    m_interface->m_compoConnexe_text.set_message(Connexe);
    m_interface->m_compoConnexe_text.set_pos(300,700);
}


void Graph::AffichageGraphReduit(int compteur)
{
    std::vector<std::vector<bool> > CompoConnexe = toutesLesComposantesFortementConnexes();
    bool idem;
    std::vector <std::vector<bool> > tmp;
    Graph GrapheReduit;
    int x = 0,y = 0,N = 0, division = 0;
    int ii = 0;


    GrapheReduit.m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    tmp.push_back(CompoConnexe[0]);

    ///On compte le nombre de composantes fortements connexes
    for(int i = 1; i < m_ordre; i++)
    {
        idem = false;
        for(unsigned int j = 0; j< tmp.size(); j++)
        {
            if(CompoConnexe[i] == tmp[j])
                idem = true;
        }
        if(idem == false)
            tmp.push_back(CompoConnexe[i]);
    }

    //for(int i = 0 ; i<tmp.)


    ///AFFICHAGE DES SOMMETS
    for(int i = 0 ; i<tmp.size(); i++)
    {
        division = 0;
        N = 0;
        x = 0;
        y = 0;
        for(int j = 0; j< tmp[i].size(); j++)
        {

            if(tmp[i][j] == true)
            {
                N = N+ m_vertices[j].m_N;
                x = x+m_vertices[j].m_x;
                y = y + m_vertices[j].m_y;
                division++;
            }
        }
        x = x / division;
        y = y / division;
        GrapheReduit.add_interfaced_vertex(i, N, x, y, "Renard.bmp");
        ii++;
    }

    ///AFFICHAGE DES ARCS
    std::vector<std::vector<int> > areteReduitEntrant;
    std::vector < std::vector < int > > mat (ii);
    areteReduitEntrant = mat;
    bool existance = false;

    for(int x = 0; x < tmp.size(); x++)
    {
        for(int y = 0; y< tmp[x].size() ; y++)
        {
            if(tmp[x][y] == true) //on entre dans une composante connexe (la xième) et on trouve un sommet connexe (le jième)
            {
                for(int w = 0; w < m_vertices[y].m_in.size(); w++)
                {
                    existance = false;
                    std::cout << " areteReduitEntrant[x].size() = " << areteReduitEntrant[x].size() << std::endl;
                    for(int z= 0; z<areteReduitEntrant[x].size();z++)
                    {
                        std::cout << " m_edges[m_vertices[y].m_in[w]].m_from  = " << m_edges[m_vertices[y].m_in[w]].m_from  << std::endl;
                        std::cout << "areteReduitEntrant[x][z] = " << areteReduitEntrant[x][z] << std::endl;
                        if(m_edges[m_vertices[y].m_in[w]].m_from != areteReduitEntrant[x][z])
                        {
                            std::cout << "JE SUIS LA " << std::endl;
                            areteReduitEntrant[x].push_back(m_edges[m_vertices[y].m_in[w]].m_from);
                        }

                    }
                }
            }
        }
    }

    std::cout << "ARETE REDUIT ENTRANT : " << std::endl;
    for(int i = 0; i<areteReduitEntrant.size(); i++)
    {
        for(int j = 0; j < areteReduitEntrant[i].size(); j++)
        {
            std::cout << areteReduitEntrant[i][j] << " ";
        }
        std::cout << std::endl;
    }



    if(compteur < 400)
    {
        std::cout << "compteur = " << compteur << std::endl;
        GrapheReduit.update();
        grman::mettre_a_jour();
    }

        //blit(GrapheReduit, screen, 0 ,0, 224 , 128, 800, 600);

}
