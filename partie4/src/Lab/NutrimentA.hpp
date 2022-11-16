#pragma once
#include "Nutriment.hpp"



class NutrimentA : public Nutriment
{
public:

    //constructeur (identique a celui de la superclass nutriment ) faut-il le marquer ?
    NutrimentA (Quantity q, Vec2d c);

    ~NutrimentA() = default;


private:
    // methode getconfig specifique au nutrimentB
    j::Value const& getConfig() const;
};



