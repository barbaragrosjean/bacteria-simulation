#include "FriendlyBacterium.hpp"

#include "../Application.hpp"
#include "../Random/Random.hpp"

#include "NutrimentA.hpp"
#include "NutrimentB.hpp"

FriendlyBacterium::FriendlyBacterium(Vec2d position)
    : Bacterium(uniform(getConfig()["energy"]["min"].toDouble(), getConfig()["energy"]["max"].toDouble()),
                position, Vec2d::fromRandomAngle(), uniform(getConfig()["radius"]["min"].toDouble(), getConfig()["radius"]["max"].toDouble()),
                getConfig()["color"])
    , lastTumbleClock(sf::Time::Zero)
    , helpClock(sf::Time::Zero)
{
    addProperty("speed", MutableNumber::positive(getConfig()["speed"]));
    addProperty("tumble better", MutableNumber::positive(getConfig()["tumble"]["better"]));
    addProperty("tumble worse", MutableNumber::positive(getConfig()["tumble"]["worse"]));
    addProperty("help delay", MutableNumber::positive(getAppConfig()["help"]["delay"]["generation"]));

    getAppEnv().increaseNumberBacteria(s::FRIENDLY_BACTERIA);
}

FriendlyBacterium::~FriendlyBacterium()
{
    getAppEnv().decreaseNumberBacteria(s::FRIENDLY_BACTERIA);
    getAppEnv().changeLenghtLife(lenghtLife, s::FRIENDLY_BACTERIA, false);
}

void FriendlyBacterium:: move(sf::Time dt)
{
    double ancien_score(getAppEnv().getPositionScore(centre));

    //calcul nouvelle position et vitesse dans la struct
    DiffEqResult nouvelles(stepDiffEq(centre, getSpeedVector(), dt, force));

    double nouveau_score(getAppEnv().getPositionScore(nouvelles.position));

    //probabilite de basculer qui depend des score: on dÃ©finit la prob selon une formule qui dÃ©pend du score

    double prob = probTumble(nouveau_score >= ancien_score);

    if(bernoulli(prob)) {

        nouvelles.speed = getProperty("speed").get() *best_direction();
        lastTumbleClock = sf::Time::Zero;
    }

    Vec2d ancienne_position(centre);

    basculement(nouvelles.position, nouvelles.speed);

    //calcul de l'Ã©nergie
    consumeEnergy(distance(ancienne_position, centre) * getEnergieDepensee());
}

double FriendlyBacterium::probTumble(bool better)
{
    if(better) {
        return 1 - exp(-lastTumbleClock.asSeconds()/getProperty("tumble better").get());
    } else {
        return 1 - exp(- lastTumbleClock.asSeconds()/getProperty("tumble worse").get());
    }
}

FriendlyBacterium* FriendlyBacterium::clone() const
{
    FriendlyBacterium* nouvelle(new FriendlyBacterium(*this));
    nouvelle->lenghtLife = sf::Time::Zero;

    getAppEnv().increaseNumberBacteria(s::FRIENDLY_BACTERIA);
    return nouvelle;
}

void FriendlyBacterium::update(sf::Time dt)
{
    Bacterium::update(dt);
    lastTumbleClock += dt;
    helpClock += dt;

    /*conditions pour générer un help:
     * 1) le temps helpClock doit être passé
     * 2) il doit avoir au moins une certaine énérgie */

    if(helpClock.asSeconds() >= getProperty("help delay").get()
       and energie >= getConfig()["energy"]["help"]["min"].toDouble()) {
        helpClock = sf::Time::Zero;
        getAppEnv().addHelp(centre);

        //ceci lui demande de l'énergie
        consumeEnergy(getConfig()["energy"]["help"]["delivery"].toDouble());
    }

    getAppEnv().changeLenghtLife(dt, s::FRIENDLY_BACTERIA, true);

}

void FriendlyBacterium::drawOn(sf::RenderTarget &target) const
{
    Bacterium::drawOn(target);

    //dessin de la specificité: croix rouge

    sf::RectangleShape ligneHor(Vec2d(rayon*4/3, rayon/3));
    sf::RectangleShape ligneVert(Vec2d(rayon/3, rayon*4/3));

    ligneHor.setPosition(centre - Vec2d(rayon*4/6, rayon/6));
    ligneVert.setPosition(centre - Vec2d(rayon/6, rayon *4/6));
    ligneHor.setFillColor(sf::Color::Red);
    ligneVert.setFillColor(sf::Color::Red);

    target.draw(ligneHor);
    target.draw(ligneVert);
}

Vec2d FriendlyBacterium::getSpeedVector()
{
    return DirectionDeplacement*getProperty("speed").get();
}

j::Value& FriendlyBacterium::getConfig() const
{
    return getAppConfig()["friendly bacterium"];
}

Quantity FriendlyBacterium::eatableQuantity(NutrimentA& nutriment)
{
    return nutriment.eatenBy(*this);
}

Quantity FriendlyBacterium:: eatableQuantity(NutrimentB& nutriment)
{
    return nutriment.eatenBy(*this);
}
