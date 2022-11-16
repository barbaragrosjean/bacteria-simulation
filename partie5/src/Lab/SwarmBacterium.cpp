#include "SwarmBacterium.hpp"
#include "Swarm.hpp"
#include "../Application.hpp"
#include "../Random/Random.hpp"

#include "NutrimentA.hpp"
#include "NutrimentB.hpp"

SwarmBacterium::SwarmBacterium(const Vec2d &position, Swarm* groupe)
    : Bacterium(uniform(getConfig()["energy"]["min"].toDouble(), getConfig()["energy"]["max"].toDouble()),
                position, Vec2d::fromRandomAngle(), uniform(getConfig()["radius"]["min"].toDouble(), getConfig()["radius"]["max"].toDouble()),
                groupe->getColor())
    , groupe(groupe)
{
    //ajoute la bactérie a son swarm
    groupe->addSwarmBacterium(this);

    getAppEnv().increaseNumberBacteria(s::SWARM_BACTERIA);

}

SwarmBacterium::~SwarmBacterium()
{
    groupe->deleteSwarmBacterium(this);
    getAppEnv().decreaseNumberBacteria(s::SWARM_BACTERIA);
}

void SwarmBacterium::drawOn(sf::RenderTarget& target) const
{
    Bacterium::drawOn(target);

    if(isLeader() and isDebugOn()) {
        auto border = buildAnnulus(centre, 30, sf::Color::Red, 4);
        target.draw(border);
    }
}

SwarmBacterium* SwarmBacterium::clone() const
{
    getAppEnv().increaseNumberBacteria(s::SWARM_BACTERIA);

    SwarmBacterium* nouvelle = new SwarmBacterium(*this);
    groupe->addSwarmBacterium(nouvelle);

    return nouvelle;
}

void SwarmBacterium::move(sf::Time dt)
{
    if(groupe->hasLeader()) {
        //calcul nouvelle position et vitesse dans la struct
        DiffEqResult nouvelles(stepDiffEq(centre, getSpeedVector(), dt, groupe->getforce()));

        if (isLeader()) {
            nouvelles.speed = getConfig()["speed"]["initial"].toDouble() * best_direction();
        }

        Vec2d ancienne_position(centre);

        basculement(nouvelles.position, nouvelles.speed);

        //calcul de l'énergie
        consumeEnergy(distance(ancienne_position, centre) * getEnergieDepensee());
    }
}

Vec2d SwarmBacterium::getSpeedVector() const
{
    return DirectionDeplacement*getConfig()["speed"]["initial"].toDouble();
}


j::Value& SwarmBacterium::getConfig() const
{
    return getAppConfig()["swarm bacterium"];
}

Quantity SwarmBacterium::eatableQuantity(NutrimentA& nutriment)
{
    return nutriment.eatenBy(*this);
}

Quantity SwarmBacterium:: eatableQuantity(NutrimentB& nutriment)
{
    return nutriment.eatenBy(*this);
}

bool SwarmBacterium::isLeader() const
{
    return (groupe->getPositionLeader() == centre);
}
