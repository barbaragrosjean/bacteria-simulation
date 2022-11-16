#pragma once

#include "Graph.hpp"
#include "../Interface/Updatable.hpp"


struct Graphe_Libelle {
    std::unique_ptr<Graph> graph;
    std::string title;
};

class Stats : public Drawable, public Updatable
{
public:

    Stats( );

    ~Stats() = default;

    void drawOn(sf::RenderTarget &target) const override final;

    void update(sf::Time dt) override final;

    //retourne le title de l'actif
    std::string getCurrentTitle() const;

    //incremente/decremente l'id actif de 1
    void next();
    void previous();

    //purge des graphes
    void reset();

    //ajout d'un graphe à l'ensemble
    void addGraph(unsigned int identificateur, std::string title,
                  std::vector<std::string> const& series, double min, double max, Vec2d size);

    //change l'identifiant actif
    void setActive(int id);


private:
    /* pour modeliser l'ensemble des graphes, qui sont associés à des libellés,
     * et qui doivent avoir une valeur, on a une map avec id et struct de graph + nom */

    std::map<unsigned int, Graphe_Libelle > ensemble;

    //identifiant actif
    unsigned int id_actif;

    //compteur depuis le dernier refresh
    sf::Time refreshClock;
};

