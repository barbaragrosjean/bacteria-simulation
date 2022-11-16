#include "Swarm.hpp"
#include "Application.hpp"
#include "SwarmBacterium.hpp"

#include <vector>

Swarm::Swarm(std::string id)
    :identificateur(id),  leader(nullptr), force(id)
{};

Swarm::~Swarm()
{
    for (auto& bac: bacteries) {

        bac = nullptr;
    }
    bacteries.clear();
}

void Swarm::update(sf::Time dt)
{
    //recherche ou actualisation du leader (indépendant du temps)
    leader = bestScore();
}

void Swarm::addSwarmBacterium(SwarmBacterium* bacterium)
{
    bacteries.push_back(bacterium);
    leader = bestScore();
}

void Swarm::deleteSwarmBacterium(SwarmBacterium* bacterium)
{
    //on elimine cet bacterie de son swarm
    bacteries.erase(std::remove(bacteries.begin(), bacteries.end(), bacterium), bacteries.end());

    leader=bestScore();
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
    MutableColor color(getAppConfig()["swarms"][identificateur]["color"]);
    return color;
}

void Swarm::setLeader(SwarmBacterium* futur)
{
    leader = futur;
}

SwarmForce Swarm::getforce() const
{
    return force;
}

bool Swarm::hasLeader() const
{
    return (leader != nullptr);
}

SwarmBacterium* Swarm::bestScore() const
{
    if (bacteries.empty()) return nullptr;

    SwarmBacterium* best(bacteries[0]);
    for(auto& bact: bacteries) {
        if(getAppEnv().getPositionScore(best->getPosition()) < getAppEnv().getPositionScore(bact->getPosition())) {
            best = bact;
        }
    }
    return best;
}
