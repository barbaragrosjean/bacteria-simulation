#include "NutrimentB.hpp"
#include "./Application.hpp"



NutrimentB :: NutrimentB (Quantity q, Vec2d c)
    : Nutriment(q, c)
{};


// methode getconfig
j::Value const& NutrimentB:: getConfig() const
{
    return getAppConfig()["nutriments"]["B"];
}


