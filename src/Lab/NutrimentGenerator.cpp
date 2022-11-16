#include "NutrimentGenerator.hpp"

#include "../Application.hpp"
#include "../Random/Random.hpp"

#include "NutrimentA.hpp"
#include "NutrimentB.hpp"

NutrimentGenerator :: NutrimentGenerator()
{
    reset();
};

void NutrimentGenerator:: update(sf::Time dt)
{
    compteur += dt;

    //on génére des nutriments pas tous les dt
    if (compteur > sf::seconds(getShortConfig().generator_nutriment_delay)) {
        //remettre le compteur a 0
        compteur = sf::Time::Zero;

        // position aléatoire
        Vec2d centre(getApp().getLabSize()/2);
        Vec2d sigma(getApp().getLabSize()/4);

        Vec2d position(normal(centre.x,sigma.x*sigma.x),normal(centre.y,sigma.y*sigma.y));

        //test de la probabilité de générer A ou B
        if  (bernoulli(getShortConfig().generator_nutriment_prob)) {
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

