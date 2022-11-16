#pragma once
#include "Nutriment.hpp"


class NutrimentB : public Nutriment
{
public:

    //constructeur (identique a celui de la superclass nutriment ) faut-il le marquer ?
    NutrimentB (Quantity q, Vec2d c);

    ~NutrimentB() = default;



private:
    // methode getconfig specifique au nutrimentB
    j::Value const& getConfig() const ;
};



