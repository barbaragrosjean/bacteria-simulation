#include "Swarm.hpp"
#include "Application.hpp"
#include "SwarmBacterium.hpp"
//#include "../Utility/SwarmForce.hpp"

#include <vector>

Swarm::Swarm(std::string id)
    :force(id), identificateur(id), leader(nullptr)
{};

Swarm::~Swarm()
{
    //quand un groupe disparait il faut que cela entraine la mort des bacteries du groupe
    //on supprime les zones mémoires allouées et on met les ptr a nullptr
    for (auto* bac: bacteries) {
        delete bac;
        bac = nullptr;

    }
    bacteries.clear();
}

void Swarm::update(sf::Time dt)
{

    //test si les bacteries ont été éliminé auparavant (dans update de PetriDish)
    for(auto& bac: bacteries) {
        if (bac==NULL) {
            bac= nullptr;
        }
    }
    //efface les nullptr
    bacteries.erase(std::remove(bacteries.begin(), bacteries.end(), nullptr), bacteries.end());

    //test si il n'y a pas de leader et si le swarm n'est pas vide
    if(!isLeader() and bacteries.size() != 0) {
        leader = bacteries[0];
        for(auto b: bacteries) {
            if(getAppEnv().getPositionScore(b->getPosition()) >= getAppEnv().getPositionScore(leader->getPosition())) {
                leader = b;
            }
        }
    }

    //changement de leader s'il y a un leader
    if(isLeader()) {
        for(auto b : bacteries) {
            if(getAppEnv().getPositionScore(b->getPosition()) >= getAppEnv().getPositionScore(leader->getPosition())) {
                leader = b;
            }
        }
    }
}


void Swarm::addSwarmBacterium(SwarmBacterium* bac)
{
    bacteries.push_back(bac);
}


void Swarm::deleteSwarmBacterium(SwarmBacterium* bac)
{
    //il faut eliminer la bacterie du groupe, ce sera la petridish qui desallouera la memoire
    for(auto* bacterie : bacteries) {
        if (bacterie == bac) {
            bacterie = nullptr;
        }
    }
    bacteries.erase(std::remove(bacteries.begin(), bacteries.end(), nullptr), bacteries.end());

    //dans le cas ou le leader est delete (mort) nous mettons le leader a nullptr
    if(leader == NULL) {
        leader = nullptr;
    }

}


Vec2d Swarm::getPositionLeader() const
{
    return leader->getPosition();
}

std::string Swarm::getIdentificateur() const
{
    return identificateur;
}


MutableColor Swarm::getColor() const
{
    MutableColor col(getAppConfig()["swarms"][identificateur]["color"]);
    return col;
}

void Swarm::setLeader(SwarmBacterium* futur)
{
    leader = futur;
}

SwarmForce Swarm::getforce() const
{
    return force;
}

bool Swarm::isLeader() const
{
    return (leader != nullptr);
}
