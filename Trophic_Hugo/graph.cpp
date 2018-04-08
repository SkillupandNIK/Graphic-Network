#include "graph.h"
#include <fstream>
#include <math.h>
#include <list>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{

    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
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


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();

//    for(unsigned int i = 0; i < m_in.size(); i++)
//        std::cout << "aretes entrantes " << i << " = " << m_in[i] << std::endl;
//    for(unsigned int i = 0; i < m_out.size(); i++)
//        std::cout << "aretes sortantes " << i << " = " << m_out[i] << std::endl;
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
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

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
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


/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
void Graph::make_example(std::string nom_fichier)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
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
    std::cout << "ordre = " << ordre << std::endl;
    m_ordre = ordre;
    std::cout << "m_ordre = " << m_ordre<< std::endl;

    for(int i = 0 ; i < ordre; i++)
    {
        Admin >> numero;
        Admin >> N;
        Admin >> x;
        Admin >> y;
        Admin >> nom_Image;
        add_interfaced_vertex(numero, N, x, y, nom_Image.c_str());
        m_vertices.at(numero).m_nom_image = nom_Image;
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


    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
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

/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
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

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
    m_vertices[idx].m_N = value;
    m_vertices[idx].m_x = x;
    m_vertices[idx].m_y = y;
}

/// Aide � l'ajout d'arcs interfac�s
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

    /// OOOPS ! Prendre en compte l'arc ajout� dans la topologie du graphe !
    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}


void Graph::supprimer_graphe()
{
    ///r�initialisation et suppression de l'interface
     m_interface = nullptr;
     m_interface.reset();
     std::cout << "Interface de " << "entrer_nom" << " supprim�e." << std::endl;

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
    std::cout << "Interface de " << "name_vertex" << " supprim�e." << std::endl;

    if (parametre)
    {
        //supprimer dans la m�moire vive !!
    }

}


/// eidx index of edge to remove
void Graph::test_remove_edge(int eidx)
{
    /// r�f�rence vers le Edge � enlever
    /// on parcourt la map jusqu'� l'endroit de l'indice rentr� en param�tre de la fonction
    Edge &remed=m_edges.at(eidx);

    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

    /// test : on a bien des �l�ments interfac�s
    if (m_interface && remed.m_interface)
    {
        /// Ne pas oublier qu'on a fait �a � l'ajout de l'arc :
        // EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        // m_interface->m_main_box.add_child(ei->m_top_edge); */
        // m_edges[idx] = Edge(weight, ei); */
        /// Le new EdgeInterface ne n�cessite pas de delete car on a un shared_ptr
        /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

    /// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !
    /// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

    /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
    /// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface
    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );

    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
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


//inspir� du livret de polycopi� du cours de th�orie des graphes 2017-2018
std::vector<bool> Graph::uneComposanteFortementConnexe(int s)
{
    //std::cout << "On rentre dans 'Une composante Fortement Connexe'" << std::endl;

    ///variables locales
    std::vector<bool> compo1, compo2; //composantes connexes directes partant de s et indirectes arrivant � s
    std::vector<bool> compo; // composantes fortement connexes : compo1 inter compo2 � retourner
    std::vector<bool> marques1, marques2;// tableau de marquage
    int x; //numero de sommets interm�diaires des composantes connexes
    bool ajoute1 = true,ajoute2 = true; ; //bool�en indiquant si une nouvelle composante connexe est ajout�

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

    ///Recherche des composantes connexes partant de s � ajouter dans compo1

    ///COMPO1
    while(ajoute1==true)
    {
        ajoute1 = false; //� chaque tour, recherche d'une nouvelle composante connexe � ajouter
        ///Pour tous les sommets x non marqu�s et connect�s en partant de s
        ///Marquer chaque sommet x et connecter les sommets non marqu�s y adjacents � x
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
                        ajoute1 = true; //nouvelle composante connexe ajout�e
                    }
                }
            }
        }
    }


    ///COMPO2
    while(ajoute2 == true)
    {
        ajoute2 = false;

        for(x= 0; x<m_ordre; x++)
        {
        ///On recherche des composantes connexes arrivant � s � ajouter dans compo2
            if(marques2[x]==false && compo2[x]==true)
            {
                //std::cout << "On rentre dans if(marques[x]==false && compo2[x]) " << std::endl;
                marques2[x] = true;
                //std::cout << "On met marques[x] � true : marques2["<< x<<"] = " << marques2[x] << std::endl;
                    for(const auto& elem : m_edges)
                    {
                        if(elem.second.m_to == x && marques2[elem.second.m_from] == false)
                        {
                            compo2[elem.second.m_from] = true;
                            ajoute2 = true;
                        }

                    }

                }
            }
        }
    for(int i=0; i<m_ordre; i++)
    {
        compo[i]=compo1[i] & compo2[i];

    }

    ///On retourne la composante fortement connexe compo
    return compo;
}

std::vector<std::vector<bool> > Graph::toutesLesComposantesFortementConnexes()
{
    ///Variables locales
    //std::cout << "Graph 1 " << std::endl;
    std::vector<std::vector<bool> >  tabc (m_ordre); //tableau des composantes fortement connexe � retourner
    std::vector<bool> marques; //tableau qui indique si un sommet est marqu� ou pas
    //std::cout << "Graph 2 " << std::endl;
    ///Allocation et initialisation des tableaux
    for(int i = 0; i < m_ordre; i++)
    {
        marques.push_back(false);
        for(int j = 0; j < m_ordre; j++)
            tabc[i].push_back(false);

    }
    //std::cout << "Graph 3 " << std::endl;

    ///Pour tous les sommmets non marqu�s
    ///Recherche de la composante fortement connexe de x
    ///Marquer chaque sommet x et marquer les sommets y connect�s � x et non marqu�s
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

    ///On cr�er une boite de texte qui va afficher le nombre de composantes fortement connexe � l'�cran
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
        GrapheReduit.add_interfaced_vertex(i, N, x, y, "animaux.bmp");
        ii++;
    }

    ///AFFICHAGE DES ARCS
    std::vector<std::vector<int> > areteReduitEntrant (tmp.size());
    bool existance = false;
    std::vector<int> ArcsGris; //vecteur d'indice d'arcs qui ne relie pas deux sommets qui sont fortements connexes entre eux


    ///Remplissage du vecteur "ArcsGris"
    for(int i = 0 ; i <m_edges.size(); i++)
    {
        if(m_edges.at(i).m_interface->m_top_edge.get_color_fleche() == GRISSOMBRE)
            ArcsGris.push_back(i);
    }

    ///Remplissage de areteReduitEntrant
    for(int i = 0; i < tmp.size(); i++)
    {
        for(int j = 0; j < tmp[i].size(); j++) //On parcourt tmp
        {
            if(tmp[i][j]==true) //si un sommet (j) appartient � la composantes fortements connexe qu'on regarde (tmp[i])
            {
                for(int x  = 0; x<ArcsGris.size();x++) //On parcours les arcs gris
                {
                    if(m_edges.at(ArcsGris[x]).m_from == j) //Si le sommet j � une relation forte avec un autre sommet (m_edges.at(ArcsGris[x]).m_to)
                    {
                        if(areteReduitEntrant[i].size()==0)//Si areteReduitEntrant est vide
                            areteReduitEntrant[i].push_back(m_edges.at(ArcsGris[x]).m_to);//on ajoute le sommet l� o� il va
                        else //si areteReduitEntrant[x] n'est pas vide
                        {
                            existance = false;
                            for(int y = 0; y < areteReduitEntrant[i].size(); y++)//on parcours le vecteur de areteReduit[x]
                            {
                                if(areteReduitEntrant[i][y] == m_edges.at(ArcsGris[x]).m_to) //on v�rifie que le sommet n'est pas d�j� �crit
                                    existance =true;
                            }
                            if(existance == false) //si le sommet n'est pas deja �crit dans areteReduitEntrant[x]
                                areteReduitEntrant[i].push_back(m_edges.at(ArcsGris[x]).m_to);//on ajoute le sommet l� o� il va
                        }
                    }
                }
            }
        }
    }

    ///Creation des arcs
    int numero = 0;
    for(int i = 0 ; i < areteReduitEntrant.size(); i++)//On parcours areteReduitEntrant
    {
        for(int j = 0 ; j < areteReduitEntrant[i].size(); j++)
        {
            for(int x = 0 ; x < tmp.size(); x++) //On parcours tmp
            {
                for(int y = 0 ; y < tmp[x].size(); y++)
                {
                    if(tmp[x][y] == true && y == areteReduitEntrant[i][j])//On trouve le sommet r�duit d'arriv�
                    {
                        GrapheReduit.add_interfaced_edge(numero, i ,x, 1);//On cr�er l'arc
                        numero ++;
                    }
                }
            }
        }
    }
    if(compteur < 200)
    {
        GrapheReduit.update();
        grman::mettre_a_jour();
    }
}

//source : http://www.commentcamarche.net/forum/affich-4567474-algo-affichage-combinaison-de-p-elemt-parmi-n
std::vector<std::vector<int> > Graph::rechercheDesKuplet(int p)
{
    std::vector<std::vector<int> > vec_k_uplet;
    std::vector<int> combination;
    int n= m_ordre;
    int j;
    //p = 3;
    // Initialisation de combinaison
    //std::cout << "coucou 1" << std::endl;
    for (int i=0;i<n;i++)
    {
        combination.push_back(i);
    }
    //std::cout << "coucou 2" << std::endl;
    // Remplissage de la vec_k_uplet
    for (int i=0;i<(int) pow((double)n, (double)p);i++)
    {
        std::vector<int> tempon;
        //std::cout << "pow((double)n, (double)p) = " << pow((double)n, (double)p) << std::endl;
        //std::cout << "coucou 3" << std::endl;
        for (j=0;j<p;j++)
        {
            //std::cout << "coucou 4" << std::endl;
            //vec_k_uplet[i][j] = combination[(i/(int )pow((double )n,(double )(p-(j+1))))%n];
            tempon.push_back(combination[(i/(int )pow((double )n,(double )(p-(j+1))))%n]);
            //std::cout << tempon[j] << " ";
        }
        std::sort(tempon.begin(), tempon.end());
        tempon.erase(std::unique(tempon.begin(), tempon.end()), tempon.end());

//        std::vector<int>::iterator ot;
//        ot = std::unique (tempon.begin(), tempon.end());   // 10 20 30 20 10 ?  ?  ?  ?
//        tempon.resize( std::distance(tempon.begin(),ot) );
        bool existance = false;
        if(tempon.size() == p)
        {

            for(int x = 0; x < vec_k_uplet.size(); x++)
            {
                if(tempon == vec_k_uplet[x])
                    existance = true;
            }

        }
        if(existance == false)
            vec_k_uplet.push_back(tempon);
        //std::cout << std::endl;
    }
//    std::vector<std::vector<int> >::iterator it;
//    it = std::unique (vec_k_uplet.begin(), vec_k_uplet.end());   // 10 20 30 20 10 ?  ?  ?  ?
//    vec_k_uplet.resize( std::distance(vec_k_uplet.begin(),it) );
    vec_k_uplet.erase(std::unique(vec_k_uplet.begin(), vec_k_uplet.end()), vec_k_uplet.end());

//    std::cout << "COUCOU AVANT BOUCLE" << std::endl;
    for(int i = 0; i < vec_k_uplet.size(); i++)
    {
        if(vec_k_uplet[i].size() < p)
        {
//            std::cout << "COUCOU DANS BOUCLE" << std::endl;
//            std::cout << "vec_k_uplet[i].size() = " << vec_k_uplet[i].size() << std::endl;
            vec_k_uplet.erase(vec_k_uplet.begin()+i);
        }

    }

    std::cout << "COMBINAISON" << std::endl;
    for(int i = 0; i < vec_k_uplet.size(); i++)
    {
        for(int j = 0; j< vec_k_uplet[i].size(); j++)
        {
            std::cout << vec_k_uplet[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return vec_k_uplet;
}


//source : https://www.geeksforgeeks.org/depth-first-search-or-dfs-for-a-graph/
std::vector<bool> Graph::RechercheUtileConnexe(int actuel, std::vector<bool> marquage)
{
    //std::cout << "Sommet actuel : " << actuel << std::endl;
    marquage[actuel] = true;
    std::vector<int> actuelAdjacent;

//    std::cout << "taille marquage = " << marquage.size() << std::endl;
//    std::cout << "MARQUAGE : " << std::endl;
//    for(int i = 0 ; i<marquage.size(); i++)
//    {
//        //if(marquage[i] == false)
//        //    connexite = false;
//        std::cout << marquage[i] << " ";
//    }
//    std::cout << std::endl;

    ///On rempli un vecteur avec tous les sommets adjacents du sommet actuel
    //on rajoute les predecesseurs

    for(int i =0; i < m_vertices.at(actuel).m_in.size(); i++)
    {
        //std::cout << "m_edges[m_vertices.at(actuel).m_in[i]].m_from = " << m_edges[m_vertices.at(actuel).m_in[i]].m_from << std::endl;
        actuelAdjacent.push_back(m_edges[m_vertices.at(actuel).m_in[i]].m_from);
    }
    //on rajoute les successeurs
    for(int i =0; i < m_vertices.at(actuel).m_out.size(); i++)
    {
        actuelAdjacent.push_back(m_edges[m_vertices.at(actuel).m_out[i]].m_to);
    }
//    std::cout << "Actuel Adjacent : " << std::endl;
//    for(int i = 0 ; i < actuelAdjacent.size(); i++)
//        std::cout << actuelAdjacent[i] << " ";
//    std::cout << std::endl;

    ///On parcours les sommets en les marquants
    for(int i = 0; i < actuelAdjacent.size() ; i++)
    {
        if(marquage[actuelAdjacent[i]] == false)
            {
                //std::cout << "Sommet suivant : " << actuelAdjacent[i] << std::endl;
                marquage = RechercheUtileConnexe(actuelAdjacent[i], marquage);
            }
    }



    return marquage;
}

bool Graph::RechercheConnexe(int ordreGraphePrecedent)
{
    bool connexite = true;
    std::vector<bool> marquage;
    for(int i = 0; i <ordreGraphePrecedent; i++)
    {
        marquage.push_back(false);
        if(m_vertices.at(i).suppressionConnexe == true)
            marquage[i] = true;
    }


    std::cout << "MARQUAGE : " << std::endl;
    for(int i = 0 ; i<marquage.size(); i++)
    {
        std::cout << marquage[i] << " ";
    }
    std::cout << std::endl;

    bool trouve = false;
    int premierSommet = 0;
    for(int i=0 ; i < ordreGraphePrecedent; i++)
    {
        if(trouve == false && m_vertices[i].suppressionConnexe == false)
        {
            premierSommet= i;
        }
    }
    marquage = RechercheUtileConnexe(premierSommet, marquage);

    for(int i = 0 ; i<marquage.size(); i++)
    {
        if(marquage[i] == false)
            connexite = false;
    }

    return connexite;
}

void Graph::k_connexite()
{
    std::vector< std::vector< int> > combinaison;
    bool affichage = false;

    while(affichage == false) //tant qu'on a pas afficher des graphes
    {
        for(int i = 1; i < m_ordre; i++) //On fait une boucle for
        {
            combinaison = rechercheDesKuplet(i); // on  regarde les combinaison possible de i(variant de 1 � m_ordre) dans n( vaut m_ordre-1)
            for(int x = 0; x< combinaison.size(); x++)
            {
                std::cout << "NOUVEAU GRAPHE " << std::endl;
                std::cout << "Sommet du graphe : ";
                int ordre = 0;
                Graph GraphNonConnexe;//on cr�er un nouveau graphe
                GraphNonConnexe.m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

                for(int sommet = 0; sommet < m_ordre; sommet++)// On parcours les sommets pour les cr�er
                {
                    bool aEffacer = false;
                    for(int sommetAeffacer=0; sommetAeffacer < combinaison[x].size(); sommetAeffacer++) //On regarde les sommets � effacer
                    {
                        if(sommet == combinaison[x][sommetAeffacer]) //si le sommet est � effacer
                            aEffacer = true;
                    }

                    if(aEffacer == false) //si le sommet n'est pas a supprimer, on le rajoute
                    {
                        GraphNonConnexe.add_interfaced_vertex(sommet, m_vertices.at(sommet).m_N, m_vertices.at(sommet).m_x, m_vertices.at(sommet).m_y, m_vertices.at(sommet).m_nom_image.c_str());
                        std::cout << sommet << " ";
                        ordre++;
                    }
                    else
                        GraphNonConnexe.m_vertices[sommet].suppressionConnexe = true;
                }
                GraphNonConnexe.m_ordre = ordre;

                for(int arc = 0; arc < m_edges.size(); arc++) //on parcours les arcs pour les cr�er
                {
                    bool aEnleverfrom = false;
                    bool aEnleverto = false;
                    for(int arcAeffacer=0; arcAeffacer < combinaison[x].size(); arcAeffacer++) //On regarde les sommets � effacer
                    {
                        if(combinaison[x][arcAeffacer] == m_edges.at(arc).m_from) //si le sommet d'o� vient l'arc est � supprimer
                            aEnleverfrom = true; //on met le booleen � true
                        if(combinaison[x][arcAeffacer] == m_edges.at(arc).m_to) //si le sommet o� va l'arc est � supprimer
                            aEnleverto = true; //on met le booleen � true
                    }
                    if(aEnleverfrom == false && aEnleverto == false) //si les sommets d'o� viennent et partent l'arcs ne sont pas � supprimer
                    {
                        //on cr�er l'arrete
                        GraphNonConnexe.add_interfaced_edge(arc, m_edges.at(arc).m_from, m_edges.at(arc).m_to, m_edges.at(arc).m_weight);
                    }
                }

                //On regarde s'il est connexe
                if(GraphNonConnexe.RechercheConnexe(m_ordre) == false) //s'il n'est pas connexe
                {
                    //std::cout << " GraphNonConnexe.RechercheConnexe() = " << GraphNonConnexe.RechercheConnexe() << std::endl;
                    //on l'affiche et on met le bool�en affichage � true.
                    for(int compteur = 0 ; compteur < 100; compteur++)
                    {
                        GraphNonConnexe.update();
                        grman::mettre_a_jour();
                    }
                    affichage = true;
                    std::cout << "Le graphe est " <<i << "-connexe." << std::endl;
                }
            }
            if(affichage == true)
                return;
        }


    }
}








