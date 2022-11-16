#include "TwitchingBacterium.hpp"
#include "../Application.hpp"
#include "../Random/Random.hpp"
#include "../Utility/Utility.hpp"

#include "NutrimentA.hpp"
#include "NutrimentB.hpp"

TwitchingBacterium::TwitchingBacterium(Vec2d position)
    : Bacterium(uniform(getConfig()["energy"]["min"].toDouble(), getConfig()["energy"]["max"].toDouble()),
                position, Vec2d::fromRandomAngle(), uniform(getConfig()["radius"]["min"].toDouble(), getConfig()["radius"]["max"].toDouble()),
                getConfig()["color"])
    , grip(position, rayon/4)
    , act(IDLE)
{
    addProperty("longueur max", MutableNumber::positive(getConfig()["tentacle"]["length"]));
    addProperty("vitesse tentacule", MutableNumber::positive(getConfig()["tentacle"]["speed"]));

    getAppEnv().increaseNumberBacteria(s::TWITCHING_BACTERIA);
    getAppEnv().changeParametreTwitching(getProperty("longueur max").get(), s::TENTACLE_LENGTH, getProperty("vitesse tentacule").get(), s::TENTACLE_SPEED, true);
}

TwitchingBacterium:: ~TwitchingBacterium()
{
    getAppEnv().decreaseNumberBacteria(s::TWITCHING_BACTERIA);
    getAppEnv().changeParametreTwitching(getProperty("longueur max").get(), s::TENTACLE_LENGTH, getProperty("vitesse tentacule").get(), s::TENTACLE_SPEED, false);
    getAppEnv().changeLenghtLife(lenghtLife, s::TWITCHING_BACTERIA, false);
}

void TwitchingBacterium:: drawOn(sf::RenderTarget &target) const
{
    Bacterium::drawOn(target);

    //dessin des specificités de la bactérie

    auto const ann = buildAnnulus(grip.getPosition(), grip.getRadius(), couleur.get(), grip.getRadius()/4);
    target.draw(ann);

    auto line = buildLine(centre, grip.getPosition(), couleur.get(), 1);
    target.draw(line);

}

void TwitchingBacterium::move(sf::Time dt)
{

    //on va tester plusieurs cas selon son état
    switch (act) {
    case IDLE:
        //tentacule au repos et on passe au prochain état
        act = WAIT_TO_DEPLOY;
        break;


    case WAIT_TO_DEPLOY:
        //tentacule se prepare et oriente la bacterie dans la bonne direction

        DirectionDeplacement = best_direction();
        act = DEPLOY;
        break;

    case DEPLOY:
        /*tentacule se deploie à une nouvelle position jusqu'a:
         * 1) sa longueur max
         * 2) toucher la petridish
         * 3) toucher un nutriment
        */

        if(getAppEnv().getNutrimentColliding(grip) != nullptr) {
            //touche un nutriment -> il attract
            act = ATTRACT;
        } else if(distance(centre, grip.getPosition()) >= getProperty("longueur max").get()) {
            //tentacule trop longue -> il retract
            act = RETRACT;
        } else if(getAppEnv().doesCollideWithDish(grip)) {
            //touche la paroi ou un obstacle -> il change de direction et retract
            DirectionDeplacement = -DirectionDeplacement;
            act = RETRACT;
        } else {
            //sinon c'est bon il peut continuer à bouger dans cette direction
            moveGrip(DirectionDeplacement * dt.asSeconds() * getProperty("vitesse tentacule").get());
            //elle perd de l'énergie
            consumeEnergy(getEnergieTentacule() * getProperty("vitesse tentacule").get() * dt.asSeconds());
            act = DEPLOY; //reste dans et état
        }

        break;

    case ATTRACT:
        //test si le corps de la bacterie touche un nutriment -> il mange
        if(getAppEnv().getNutrimentColliding(*this) != nullptr) {
            act = EAT;
        } else {
            //sinon il attire la bacterie vers sa position et se prepare à attaquer à nouveau
            Vec2d ancienne_position(centre);

            CircularBody::move(Vec2d(grip.getPosition() - centre).normalised() *
                               getConfig()["speed factor"].toDouble() * getProperty("vitesse tentacule").get() *dt.asSeconds());
            consumeEnergy(getEnergieMouvement() * distance(ancienne_position, centre));
            act = WAIT_TO_DEPLOY;
        }
        break;

    case RETRACT:
        //tentacule se retracte
        if(distance(centre, grip.getPosition()) <= rayon - grip.getRadius()) {
            //si on la voit plus (dans bact) on passe direct à l'état idle
            act = IDLE;
        } else {
            //sinon on retracte la tentacule d'un peu à la fois et donc consomme energie
            moveGrip(Vec2d(centre - grip.getPosition()).normalised());
            consumeEnergy(getEnergieTentacule() * getProperty("vitesse tentacule").get() * dt.asSeconds());
        }
        break;

    case EAT:
        //mange que s'il est toujours en contact
        if(getAppEnv().getNutrimentColliding(*this) == nullptr) {
            act = IDLE;
        }
        break;
    }

}

void TwitchingBacterium::update(sf::Time dt)
{
    Bacterium::update(dt);
    getAppEnv().changeLenghtLife(dt, s::TWITCHING_BACTERIA, true);
}

//méthodes pour la division

TwitchingBacterium* TwitchingBacterium:: clone() const
{
    getAppEnv().increaseNumberBacteria(s::TWITCHING_BACTERIA);
    getAppEnv().changeParametreTwitching(getProperty("longueur max").get(), s::TENTACLE_LENGTH, getProperty("vitesse tentacule").get(), s::TENTACLE_SPEED, true);

    //vecteur de deplacement du clone generé dans une direction aleatoire
    Vec2d deplacement = Vec2d::fromRandomAngle() * rayon * 1.5;

    //creation d'un twitching bacterium cloné
    TwitchingBacterium* nouvelle (new TwitchingBacterium(*this));
    nouvelle->setPosition(centre+deplacement);

    nouvelle->lenghtLife = sf::Time::Zero;

    return nouvelle;
}

//Getters
Quantity TwitchingBacterium:: getEnergieMouvement() const
{
    return getConfig()["energy"]["consumption factor"]["move"].toDouble();
}

Quantity TwitchingBacterium:: getEnergieTentacule() const
{
    return getConfig()["energy"]["consumption factor"]["tentacle"].toDouble();
}

//méthodes de tentacule
void TwitchingBacterium:: moveGrip(const Vec2d& delta)
{
    grip.move(delta);
}

//getConfig
j::Value& TwitchingBacterium::getConfig() const
{
    return getAppConfig()["twitching bacterium"];
}


Quantity TwitchingBacterium::eatableQuantity(NutrimentA& nutriment)
{
    return nutriment.eatenBy(*this);
}

Quantity TwitchingBacterium:: eatableQuantity(NutrimentB& nutriment)
{
    return nutriment.eatenBy(*this);
}

