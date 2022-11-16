#include "SimpleBacterium.hpp"

#include "../Utility/DiffEqSolver.hpp"
#include "../Application.hpp"
#include "../Random/Random.hpp"

SimpleBacterium::SimpleBacterium(Vec2d p)
    : Bacterium(uniform(getConfig()["energy"]["min"].toDouble(), getConfig()["energy"]["max"].toDouble()),
                p, Vec2d::fromRandomAngle(), uniform(getConfig()["radius"]["min"].toDouble(), getConfig()["radius"]["max"].toDouble()),
                getConfig()["color"])
    , moveNumber(uniform(0.0, PI))
    , lastTumbleClock(sf::Time::Zero)
{
    addProperty("speed", MutableNumber::positive(getConfig()["speed"]));
    addProperty("tumble better", MutableNumber::positive(getConfig()["tumble"]["better"]));
    addProperty("tumble worse", MutableNumber::positive(getConfig()["tumble"]["worse"]));
}

void SimpleBacterium:: move(sf::Time dt)
{
    moveNumber += 3* dt.asSeconds();

    double ancien_score(getAppEnv().getPositionScore(centre));

    //calcul nouvelle position et vitesse dans la struct
    DiffEqResult nouvelles(stepDiffEq(centre, getSpeedVector(), dt, force));

    double nouveau_score(getAppEnv().getPositionScore(nouvelles.position));

    //probabilite de basculer qui depend des score: on définit la prob selon une formule qui dépend du score

    double prob(0);

    if(nouveau_score >= ancien_score) {
        prob = 1 - exp(-lastTumbleClock.asSeconds()/getProperty("tumble better").get());
    } else {
        prob = 1 - exp(- lastTumbleClock.asSeconds()/getProperty("tumble worse").get());
    }

    if(bernoulli(prob)) {

        nouvelles.speed = getProperty("speed").get()*best_direction();
        lastTumbleClock = sf::Time::Zero;
    }

    Vec2d ancienne_position(centre);

    basculement(nouvelles.position, nouvelles.speed);

    //calcul de l'énergie
    consumeEnergy(distance(ancienne_position, centre) * getEnergieDepensee());
}

void SimpleBacterium::update(sf::Time dt)
{
    Bacterium::update(dt);
    lastTumbleClock += dt;
}

SimpleBacterium* SimpleBacterium::clone() const
{
    return new SimpleBacterium(*this);
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
