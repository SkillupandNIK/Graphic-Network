#include "graph.h"
#include <iostream>
#include <fstream>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    m_idx = idx;
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 10000.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    //La petite croix pour la suppression
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
void Vertex::post_update(int *x)
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();

    if (m_interface->m_supp.clicked())
        *x = m_interface->get_idx();
}


int VertexInterface::get_idx()
{
    return m_idx;
}

double Vertex::get_value()
{
    return m_value;
}

double Vertex::calcul_population()
{
    double coeff_general(0);

    for (unsigned int i = 0; i < m_in.size(); i++)
    {
        //coeff_general += (m_in[i].get_weight() * m_in[i].m_from.get_value());
    }
    return coeff_general;
}


double Edge::get_weight()
{
    return m_weight;
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
    m_slider_weight.set_range(0.0, 20.0);  // Valeurs arbitraires, à adapter...
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

    /// La boite à boutons
    m_tool_box.add_child( m_boite_de_boutons );
    m_boite_de_boutons.set_dim(75,160);
    m_boite_de_boutons.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_boite_de_boutons.set_bg_color(GRISSOMBRE);
    //m_boite_boutons.set_moveable();

    /// bouton Save
    m_boite_de_boutons.add_child( m_boutonSave );
    m_boutonSave.set_frame(0,0,78,44);
    m_boutonSave.set_bg_color(VERTFLUOSOMBRE);

    m_boutonSave.add_child(m_boutonSave_label);
    m_boutonSave_label.set_message("SAVE");

    /// bouton Delete Animal (NON UTILISE ??)
    m_boite_de_boutons.add_child( m_boutonDeleteAnimal );
    m_boutonDeleteAnimal.set_frame(0,45,78,44);
    m_boutonDeleteAnimal.set_bg_color(ROUGESOMBRE);

    m_boutonDeleteAnimal.add_child(m_boutonDeleteAnimal_label);
    m_boutonDeleteAnimal_label.set_message("SUPP Anim");

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

    std::ifstream Admin(nom_fichier.c_str());
    int ordre;
    int numero, x, y;
    double N;
    std::string nom_Image;
    Admin >> ordre;

    for(int i = 0 ; i < ordre; i++)
    {
        Admin >> numero;
        Admin >> N;
        Admin >> x;
        Admin >> y;
        Admin >> nom_Image;
        add_interfaced_vertex(numero, N, x, y, nom_Image.c_str());
    }
    /*
    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);*/

    /*/// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);*/
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

}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    int *x = new int;
    *x = -1;

    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update(x);

    for (auto &elt : m_edges)
        elt.second.post_update();

    ///reellement utile ???
    if(m_interface->m_boutonDeleteAnimal.clicked())
        std::cout << "Cliquez sur le sommet que vous souhaitez supprimer !";

    ///Supperession de Sommet
    if (*x > -1 && *x < signed (m_vertices.size()))
        test_remove_vertex(*x);
    delete x;

    //if (m_interface->
        evolution_pop();

    if(m_interface->m_boutonSave.clicked())
        Save("ChaineSauvee.txt");

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

    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}



/// eidx index of edge to remove
void Graph::test_remove_edge(int eidx)
{
    /// référence vers le Edge à enlever
    /// on parcourt la map jusqu'à l'endroit de l'indice rentré en paramètre de la fonction
    Edge remed=m_edges.at(eidx);

    //std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " de poid " << remed.m_weight << std::endl;

    /// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
//    std::cout << "AVANT : size entrant de " << remed.m_from << " : " << m_vertices[remed.m_from].m_in.size() << " : size sortant de " << remed.m_from << " : " << m_vertices[remed.m_from].m_out.size() << std::endl;
//    std::cout << "AVANT : size entrant de " << remed.m_to << " : " << m_vertices[remed.m_to].m_in.size() << " : size sortant de " << remed.m_to << " : " << m_vertices[remed.m_to].m_out.size() << std::endl;

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
//    std::cout << "APRES : size entrant de " << remed.m_from << " : " << m_vertices[remed.m_from].m_in.size() << " : size sortant de " << remed.m_from << " : " << m_vertices[remed.m_from].m_out.size() << std::endl;
//    std::cout << "APRES : size entrant de " << remed.m_to << " : " << m_vertices[remed.m_to].m_in.size() << " : size sortant de " << remed.m_to << " : " << m_vertices[remed.m_to].m_out.size() << std::endl;
//    std::cout << "APRES : taille des edges a la fin : " << m_edges.size() << std::endl;

}

void Graph::test_remove_vertex(int vidx)
{
    Vertex &remver = m_vertices.at(vidx);

//    for (unsigned int i = 0; i < remver.m_in.size(); i++)
//    {
//        test_remove_edge(remver.m_in[i]);
//        std::cout << "IN : Tour n" << i << std::endl;
//        std::cout << "IN size : " << remver.m_in.size() << std::endl;
//    }
/// IL FAUT QUE LE SOMMET ET LES ARRETES SOIENT SUPPRIMEE DANS LA MAP M_VERTICES !!!
    /*  std::map<int, Vertex>::iterator it;
      std::cout <<"sdfsdf" << std::endl;
      it=m_vertices.find(vidx);
      m_vertices.erase(0);
      std::cout <<"spppppp" << std::endl;
    */
    int i = remver.m_in.size()-1;
    while (!remver.m_in.empty())
    {
        test_remove_edge(remver.m_in[i]);
        i--;
    }

    int j = remver.m_out.size()-1;
    while (!remver.m_out.empty())
    {
        test_remove_edge(remver.m_out[j]);
        j--;
    }


    if (m_interface && remver.m_interface)
    {
        m_interface->m_main_box.remove_child (remver.m_interface->m_top_box);
        m_vertices.erase(vidx);
    }



}

void Graph::Save(std::string nom_fichier)
{
    std::ofstream Admin(nom_fichier.c_str());
    Admin << m_vertices.size() << "\n";

    for (std::map<int, Vertex>::iterator it=m_vertices.begin(); it!=m_vertices.end(); ++it)
    {
        Admin << it->first;

        Admin << " " << m_vertices[it->first].m_interface->m_label_value.get_message();
        Admin << " " << m_vertices[it->first].m_interface->m_top_box.get_posx();
        Admin << " " << m_vertices[it->first].m_interface->m_top_box.get_posy();
        Admin << " " << m_vertices[it->first].m_interface->m_img.get_pic_name() << "\n";

    }

    Admin << m_edges.size() << "\n";


    for (std::map<int, Edge>::iterator it=m_edges.begin(); it!=m_edges.end(); ++it)
    {
        Admin << it->first;
        Admin << " " << m_edges[it->first].m_from;
        Admin << " " << m_edges[it->first].m_to;
        Admin << " " << m_edges[it->first].m_weight << "\n";
    }
    Admin.close();

    std::cout << "SAUVEGARDE" <<std::endl;
}

void Graph::evolution_pop()
{
    unsigned int coeff(0), K(0);

    std::vector <int> valeurs(m_vertices.size());

    for (const auto& elem : m_vertices)
    {
        valeurs[elem.first] = acces_valeur_sommet(elem.first);

        coeff = acces_coeff(elem.first);
        K = acces_K(elem.first);
        double y = 1-elem.second.m_value/K;
        double Nt = elem.second.m_value;

        if (y < 0)
            y = 0;
        if (Nt < 0)
            Nt = 0;

        if (elem.first == 5)
        {
            std::cout << "elements : " << valeurs[elem.first] << std::endl;
            std::cout << "K : " << K << std::endl;
            std::cout << "N divisé par K = " << elem.second.m_value/K << std::endl;
            std::cout << "coeff : " << coeff << std::endl;
        }

        ///Si pas de predecesseur
        if (elem.second.m_in.empty())
            valeurs[elem.first] = Nt + (0.01 * Nt * y ) - 0.011 * Nt;
        ///Si pas de successeur
        else if (elem.second.m_out.empty())
        {
            valeurs[elem.first] = Nt + (0.001 * Nt * y ) - (0.00001 * coeff);

            std::cout << "elements : " << valeurs[elem.first] << std::endl;
            std::cout << "K : " << K << std::endl;
            std::cout << "coeff : " << coeff << std::endl;
            std::cout << "Y : " << y << std::endl;
        }
        ///Si ni successeur ni predecesseur
        else if (elem.second.m_out.empty() && elem.second.m_in.empty())
        {
            valeurs[elem.first] = Nt + (0.001 * Nt * y );
            //valeurs[elem.first] = Nt + (0.001 * Nt * y );
        }
        ///Si les 2
        else
        {
            valeurs[elem.first] = Nt + (0.001 * Nt * y ) - (0.0001 * coeff);
        }
        if (valeurs[elem.first] < 0)
                valeurs[elem.first] = 0;
    }

    for (auto& elem : m_vertices)
    {
        elem.second.m_value = valeurs[elem.first];
    }
}


int Graph::acces_valeur_sommet(int vidx)
{
    return m_vertices[vidx].m_value;
}

int Graph::acces_coeff(int vidx)
{
    int coeff(0);
    Vertex &sommet = m_vertices.at(vidx);

    for (unsigned int i = 0; i < sommet.m_in.size(); i++)
    {
        Edge &entrante = m_edges.at(sommet.m_in[i]);
        coeff = coeff + (entrante.m_weight * m_vertices.at(entrante.m_from).m_value);
    }

    return coeff;
}

int Graph::acces_K(int vidx)
{
    ///Si pas d'arete sortante : on laisse k a m_value => N(t+1) = Nt (à modifier ?)
    int K = 50 * m_vertices.at(vidx).m_value;
    Vertex &sommet = m_vertices.at(vidx);

    if (!sommet.m_out.empty())
    {
        for (unsigned int i = 0; i < sommet.m_out.size(); i++)
        {
            Edge &sortante = m_edges.at(sommet.m_out[i]);
                K = K + (sortante.m_weight * m_vertices.at(sortante.m_to).m_value);
        }
    }

    return K;
}

