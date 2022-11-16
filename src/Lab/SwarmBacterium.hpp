
#pragma once


#include "Bacterium.hpp"
//#include "../Utility/SwarmForce.hpp"

//#include "../Utility/Vec2d.hpp"

class Swarm;


class SwarmBacterium : public Bacterium
{
public:
    SwarmBacterium(const Vec2d& position, Swarm* groupe);

    ~SwarmBacterium();

    //methode de dessin
    void drawOn(sf::RenderTarget &target) const override final;

    void update(sf::Time dt) override final;



    //differencié les nutriments A et B
    Quantity eatableQuantity(NutrimentA& nutriment) override;
    Quantity eatableQuantity(NutrimentB& nutriment) override;

private:
    //groupe auquel appartient la bac
    Swarm* groupe;

    //racourcit
    j::Value& getConfig() const override;

    //true si c'est le leader du groupe auquel il appartient
    bool isLeader() const;

    Vec2d getSpeedVector() const;

    // même que simpleBac mais la nouvelle bac doit appartenir au même groupe
    SwarmBacterium* clone() const override final;

    void move(sf::Time dt) override final;
};


