#pragma once

#include "Bacterium.hpp"
#include "NullForce.hpp"

class SimpleBacterium : public Bacterium
{
public:
    SimpleBacterium(Vec2d position);

    ~SimpleBacterium();

    //dessin de sa specificite : la flagelle
    void drawOn(sf::RenderTarget &target) const override final;

    void update(sf::Time dt) override final;

    //differencié les nutriments A et B
    Quantity eatableQuantity(NutrimentA& nutriment) override;
    Quantity eatableQuantity(NutrimentB& nutriment) override;

private:

    virtual  j::Value& getConfig() const override final;

    NullForce force;

    //compteur de mouvement
    double moveNumber;

    //compteur depuis le dernier basculement
    sf::Time lastTumbleClock;

    //vitesse courante
    Vec2d getSpeedVector();

    void move(sf::Time dt) override final;

    SimpleBacterium* clone() const override final;

    //retourne la probabilité de basculer (true -> better tumble)
    double probTumble(bool better);
};


