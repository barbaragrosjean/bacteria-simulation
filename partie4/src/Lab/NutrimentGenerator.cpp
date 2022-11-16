#include "NutrimentGenerator.hpp"
#include "../Application.hpp"
#include "../Utility/Utility.hpp"
#include "../Random/Random.hpp"
#include "../Utility/Vec2d.hpp"
#include "NutrimentA.hpp"
#include "NutrimentB.hpp"

NutrimentGenerator :: NutrimentGenerator()
{
    reset();
};

void NutrimentGenerator:: update(sf::Time dt)
{
    compteur += dt;

    if (compteur > sf::seconds(getAppConfig()["generator"]
                               ["nutriment"]["delay"].toDouble())) {
        //remettre le compteur a 0
        compteur = sf::Time::Zero;

        // position aléatoire
        Vec2d centre(getApp().getLabSize()/2);
        Vec2d sigma(getApp().getLabSize()/4);

        Vec2d position(normal(centre.x,sigma.x*sigma.x),normal(centre.y,sigma.y*sigma.y));



        // retroune 1 si un nutrimentA a ete generé sinon un B
        if  (bernoulli(getAppConfig()["generator"] ["nutriment"]["prob"].toDouble()) ==1) {
            getAppEnv().addNutriment(new NutrimentA (uniform(getShortConfig().nutrimentA_min_qty,
                                     getShortConfig().nutrimentA_max_qty), position));
        } else {
            getAppEnv().addNutriment(new NutrimentB (uniform(getShortConfig().nutrimentB_min_qty,
                                     getShortConfig().nutrimentB_max_qty), position));
        }
    }

};

void NutrimentGenerator::reset()
{
    compteur=sf::Time::Zero;
}

