
#pragma once


#include "Bacterium.hpp"
//#include "../Utility/SwarmForce.hpp"

//#include "../Utility/Vec2d.hpp"

class Swarm;


class SwarmBacterium : public Bacterium
{
public:
    SwarmBacterium(Vec2d p, Swarm* gr);

    ~SwarmBacterium();

    //methode de dessin
    void drawOn(sf::RenderTarget &target) const override;

    // même que simpleBac mais la nouvelle bac doit appartenir au même groupe
    SwarmBacterium* clone() const override;

    void move(sf::Time dt) override;

    Vec2d getSpeedVector() const;



private:
    //groupe auquel appartient la bac
    Swarm* groupe;

    //racourcit
    j::Value& getConfig() const override;
};


