#pragma once
#include "Nutriment.hpp"


class NutrimentB : public Nutriment
{
public:

    //constructeur (identique a celui de la superclass nutriment )
    NutrimentB (Quantity quantity, Vec2d center);

    ~NutrimentB() = default;

    //differente facon de manger un nutriment selon le type de bacterie qui mange
    Quantity eatenBy(Bacterium& bact) override final;
    Quantity eatenBy(SimpleBacterium& bact) override final;
    Quantity eatenBy(TwitchingBacterium& bact) override final;
    Quantity eatenBy(SwarmBacterium& bact) override final;

private:
    // methode getconfig specifique au nutrimentB
    j::Value const& getConfig() const override final ;
};



