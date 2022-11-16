#pragma once

#include "Bacterium.hpp"
#include "NullForce.hpp"


class FriendlyBacterium : public Bacterium
{

public:

    FriendlyBacterium(Vec2d position);

    ~FriendlyBacterium();

    void drawOn(sf::RenderTarget &target) const override final;

    void update(sf::Time dt) override final;

    //differenciÃ© les nutriments A et B
    Quantity eatableQuantity(NutrimentA& nutriment) override;
    Quantity eatableQuantity(NutrimentB& nutriment) override;

private:

    virtual  j::Value& getConfig() const override final;

    NullForce force;

    //compteur depuis le dernier basculement
    sf::Time lastTumbleClock;

    //vitesse courante
    Vec2d getSpeedVector();

    void move(sf::Time dt) override final;

    FriendlyBacterium* clone() const override final;

    //retourne la probabilitÃ© de basculer (true -> better tumble)
    double probTumble(bool better);

    //compteur depuis le dernier help distribué
    sf::Time helpClock;

};

