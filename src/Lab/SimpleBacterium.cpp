#include "SimpleBacterium.hpp"

#include "../Application.hpp"
#include "../Random/Random.hpp"

#include "NutrimentA.hpp"
#include "NutrimentB.hpp"

SimpleBacterium::SimpleBacterium(Vec2d position)
    : Bacterium(uniform(getConfig()["energy"]["min"].toDouble(), getConfig()["energy"]["max"].toDouble()),
                position, Vec2d::fromRandomAngle(), uniform(getConfig()["radius"]["min"].toDouble(), getConfig()["radius"]["max"].toDouble()),
                getConfig()["color"])
    , moveNumber(uniform(0.0, PI))
    , lastTumbleClock(sf::Time::Zero)
{
    addProperty("speed", MutableNumber::positive(getConfig()["speed"]));
    addProperty("tumble better", MutableNumber::positive(getConfig()["tumble"]["better"]));
    addProperty("tumble worse", MutableNumber::positive(getConfig()["tumble"]["worse"]));

    getAppEnv().increaseNumberBacteria(s::SIMPLE_BACTERIA);

    //true -> increase
    getAppEnv().changeTumbleSimple(getProperty("tumble better").get(), s::BETTER, getProperty("tumble worse").get(), s::WORSE, true);
}

SimpleBacterium:: ~SimpleBacterium()
{
    getAppEnv().decreaseNumberBacteria(s::SIMPLE_BACTERIA);

    //false -> decrease
    getAppEnv().changeTumbleSimple(getProperty("tumble better").get(), s::BETTER, getProperty("tumble worse").get(), s::WORSE, false);

    getAppEnv().changeLenghtLife(lenghtLife, s::SIMPLE_BACTERIA, false);
}

void SimpleBacterium:: move(sf::Time dt)
{
    moveNumber += 3* dt.asSeconds();

    double ancien_score(getAppEnv().getPositionScore(centre));

    //calcul nouvelle position et vitesse dans la struct
    DiffEqResult nouvelles(stepDiffEq(centre, getSpeedVector(), dt, force));

    double nouveau_score(getAppEnv().getPositionScore(nouvelles.position));

    //probabilite de basculer qui depend des score: on définit la prob selon une formule qui dépend du score

    double prob = probTumble(nouveau_score >= ancien_score);

    if(bernoulli(prob)) {

        nouvelles.speed = getProperty("speed").get()*best_direction();
        lastTumbleClock = sf::Time::Zero;
    }

    Vec2d ancienne_position(centre);

    basculement(nouvelles.position, nouvelles.speed);

    //calcul de l'énergie
    consumeEnergy(distance(ancienne_position, centre) * getEnergieDepensee());
}

double SimpleBacterium::probTumble(bool better)
{
    if(better) {
        return 1 - exp(-lastTumbleClock.asSeconds()/getProperty("tumble better").get());
    } else {
        return 1 - exp(- lastTumbleClock.asSeconds()/getProperty("tumble worse").get());
    }
}

void SimpleBacterium::update(sf::Time dt)
{
    Bacterium::update(dt);
    lastTumbleClock += dt;
    takeHelp();

    getAppEnv().changeLenghtLife(dt, s::SIMPLE_BACTERIA, true);
}

SimpleBacterium* SimpleBacterium::clone() const
{
    SimpleBacterium* nouvelle(new SimpleBacterium(*this));

    nouvelle->lenghtLife = sf::Time::Zero;

    //on doit incrementer les stats
    getAppEnv().increaseNumberBacteria(s::SIMPLE_BACTERIA);
    getAppEnv().changeTumbleSimple(getProperty("tumble better").get(), s::BETTER, getProperty("tumble worse").get(), s::WORSE, true);


    return nouvelle;
}

j::Value& SimpleBacterium::getConfig() const
{
    return getAppConfig()["simple bacterium"];
}

Vec2d SimpleBacterium::getSpeedVector()
{
    return DirectionDeplacement*getProperty("speed").get();
}

void SimpleBacterium::drawOn(sf::RenderTarget &target) const
{
    Bacterium::drawOn(target);

    //dessin de la flagelle:

    auto set_of_points = sf::VertexArray(sf::LinesStrip);

    // ajout de points à l'ensemble:
    for(int i(0); i < 30; ++i) {
        float x = static_cast<float>(-i * rayon/10.0);
        float y = static_cast<float>(rayon * sin(moveNumber) * sin(2*i/10.0));
        set_of_points.append({{x, y}, couleur.get()});
    }

    //placer l'ensemble de points au bon endroit
    auto transform = sf::Transform(); //matrice de transformation

    //on va mettre les points au bon endroit pour la bacterie: ça part de son centre
    transform.translate(centre);

    //tourner la flagelle selon la direction de deplacement
    transform.rotate(rotation/DEG_TO_RAD);

    //dessin
    target.draw(set_of_points, transform);

}

Quantity SimpleBacterium::eatableQuantity(NutrimentA& nutriment)
{
    return nutriment.eatenBy(*this);
}

Quantity SimpleBacterium:: eatableQuantity(NutrimentB& nutriment)
{
    return nutriment.eatenBy(*this);
}

