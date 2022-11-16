#include "NutrimentA.hpp"
#include "./Application.hpp"

NutrimentA::NutrimentA (Quantity q, Vec2d c)
    : Nutriment(q,c)
{};


j::Value const& NutrimentA :: getConfig() const
{
    return getAppConfig()["nutriments"]["A"];
};

