#include "SwarmBacterium.hpp"
#include "Swarm.hpp"
#include "../Application.hpp"
#include "../Random/Random.hpp"


SwarmBacterium::SwarmBacterium(Vec2d p, Swarm* gr)
    : Bacterium(uniform(getConfig()["energy"]["min"].toDouble(), getConfig()["energy"]["max"].toDouble()),
                p, Vec2d::fromRandomAngle(), uniform(getConfig()["radius"]["min"].toDouble(), getConfig()["radius"]["max"].toDouble()),
                gr->getColor())
    , groupe(gr)
{
    //ajoute al bactérie a son swarm
    groupe->addSwarmBacterium(this);


}

SwarmBacterium::~SwarmBacterium()
{
    groupe->deleteSwarmBacterium(this);

}

void SwarmBacterium::drawOn(sf::RenderTarget& target) const
{
    Bacterium::drawOn(target);


    if(centre == groupe->getPositionLeader() and isDebugOn()) {
        auto border = buildAnnulus(centre, 30, sf::Color::Red, 4);
        target.draw(border);
    }

}


SwarmBacterium* SwarmBacterium::clone() const
{
    SwarmBacterium* nouvelle = new SwarmBacterium(*this);

    //est-ce que le constructeur de copie le fait tout seul comme le contructeur le fait ?
    //groupe->addSwarmBacterium(nouvelle);

    return nouvelle;

}

void SwarmBacterium::move(sf::Time dt)
{
    if(groupe->isLeader()) {
        //calcul nouvelle position et vitesse dans la struct
        DiffEqResult nouvelles(stepDiffEq(centre, getSpeedVector(), dt, groupe->getforce()));


        if (centre == groupe->getPositionLeader()) {
            nouvelles.speed = getConfig()["speed"]["initial"].toDouble() * best_direction();

        }


        Vec2d ancienne_pos(centre);
        basculement(nouvelles.position, nouvelles.speed);

        //calcul de l'énergie
        consumeEnergy(distance(ancienne_pos, centre) * getEnergieDepensee());
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

