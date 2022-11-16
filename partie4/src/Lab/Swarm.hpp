#pragma once

#include <../Utility/Vec2d.hpp>
#include <../Utility/Utility.hpp>
#include "../Interface/Updatable.hpp"
#include <vector>
#include <string>
#include "../Utility/MutableColor.hpp"
#include "SwarmForce.hpp"

//pre-déclaration de la classe SwarmBactérium

class SwarmBacterium;

//pre-declaration de la classe SwarmForce
//class SwarmForce;



class Swarm : public Updatable
{
public:

    //constructeur
    Swarm(std::string id);

    //destructeur
    ~Swarm();

    //evolution dans le temps : mettre a jour le leader
    void update(sf::Time dt) override;

    //ajouter un SwarmBacterium au groupe
    void addSwarmBacterium(SwarmBacterium* bac);

    //retirer un SwarmBacterium au groupe
    void deleteSwarmBacterium(SwarmBacterium* bac);

    //retourne la position du leader
    Vec2d getPositionLeader() const;

    //getter pour l'identificateur
    std::string getIdentificateur() const;

    //retourne la couleur du swarm
    MutableColor getColor() const;

    //set le leader
    void setLeader(SwarmBacterium* futur);

    //getter pour la force
    SwarmForce getforce() const;

    //return true s'il y a un leader
    bool isLeader() const;


private:

    std::string identificateur;

    std::vector<SwarmBacterium*> bacteries;

    //bacterie la plus proche de la source de nutriment
    SwarmBacterium* leader;

    //SwarmForce force;
    SwarmForce force;

};




