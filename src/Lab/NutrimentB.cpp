#include "NutrimentB.hpp"
#include "./Application.hpp"

#include "Bacterium.hpp"
#include "SwarmBacterium.hpp"
#include "TwitchingBacterium.hpp"
#include "SimpleBacterium.hpp"
#include "FriendlyBacterium.hpp"


NutrimentB :: NutrimentB (Quantity quantity, Vec2d center)
    : Nutriment(quantity, center)
{};


// methode getconfig
j::Value const& NutrimentB:: getConfig() const
{
    return getAppConfig()["nutriments"]["B"];
}


Quantity NutrimentB::eatenBy(Bacterium& bact)
{
    return bact.eatableQuantity(*this);
}

//ces bacteries rencontrent une resistance quand elles mangent (mangent lentement)
Quantity NutrimentB::eatenBy(SimpleBacterium& bact)
{
    return takeQuantity(bact.getMaxQuantite() / getConfig()["resistance factor"].toDouble());
}

//pour ces bacteries c'est un énergisant
Quantity NutrimentB::eatenBy(TwitchingBacterium& bact)
{
    return takeQuantity(bact.getMaxQuantite()) * getShortConfig().nutrimentB_nutritive_factor;
}

//pour ces bacteries c'est un poison
Quantity NutrimentB::eatenBy(SwarmBacterium& bact)
{
    return -takeQuantity(bact.getMaxQuantite()) * getShortConfig().nutrimentB_poison_factor;
}

Quantity NutrimentB::eatenBy(FriendlyBacterium& bact)
{
    return takeQuantity(bact.getMaxQuantite());
}
