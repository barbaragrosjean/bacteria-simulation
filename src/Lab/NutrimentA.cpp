#include "NutrimentA.hpp"
#include "./Application.hpp"

#include "Bacterium.hpp"
#include "SwarmBacterium.hpp"
#include "TwitchingBacterium.hpp"
#include "SimpleBacterium.hpp"
#include "FriendlyBacterium.hpp"


NutrimentA::NutrimentA (Quantity quanity, Vec2d center)
    : Nutriment(quanity,center)
{};


j::Value const& NutrimentA :: getConfig() const
{
    return getAppConfig()["nutriments"]["A"];
};

Quantity NutrimentA::eatenBy(Bacterium& bact)
{
    return bact.eatableQuantity(*this);
}


Quantity NutrimentA::eatenBy(SimpleBacterium& bact)
{
    return takeQuantity(bact.getMaxQuantite());
}

Quantity NutrimentA::eatenBy(TwitchingBacterium& bact)
{
    return takeQuantity(bact.getMaxQuantite());
}

Quantity NutrimentA::eatenBy(SwarmBacterium& bact)
{
    return takeQuantity(bact.getMaxQuantite());
}

Quantity NutrimentA::eatenBy(FriendlyBacterium& bact)
{
    return takeQuantity(bact.getMaxQuantite());
}

