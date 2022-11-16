
#pragma once

#include "Bacterium.hpp"
#include "../Utility/NullForce.hpp"


class SimpleBacterium : public Bacterium
{
public:
    SimpleBacterium(Vec2d p);

    ~SimpleBacterium() = default;

    void move(sf::Time dt) override;

    SimpleBacterium* clone() const override;

    //vitesse courante
    Vec2d getSpeedVector();

    //dessin de sa specificite : la flagelle
    void drawOn(sf::RenderTarget &target) const override;

    void update(sf::Time dt) override;

    //void mutate() override;

private:

    virtual  j::Value& getConfig() const override;

    NullForce force;

    //compteur de mouvement
    double moveNumber;

    //compteur depuis le dernier basculement
    sf::Time lastTumbleClock;

};


