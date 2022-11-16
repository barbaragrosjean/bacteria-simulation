#include "Lab.hpp"
#include "Application.hpp"

Lab::Lab()
    : boite(getApp().getCentre(), ((getApp().getLabSize().x)*0.95)/2)
{}


//on teste avec la methode contains de CircularBody si le body est contenu dans la PetriDish                    ??
bool Lab::contains(const CircularBody& body) const
{
    return boite.contains(body);
}

void Lab::drawOn(sf::RenderTarget& targetWindow) const
{
    boite.drawOn(targetWindow);
}

void Lab::update(sf::Time dt)
{
    boite.update(dt);
    generator.update(dt);
}

void Lab:: addNutriment(Nutriment* nutriment)
{
    boite.addNutriment(nutriment);
}

void Lab:: addBacterium(Bacterium* bacterium)
{
    boite.addBacterium(bacterium);
}


void Lab::decreaseTemperature()
{
    //enleve à la temperature actuelle le delta
    boite.setTemperature(getTemperature() - getAppConfig()["petri dish"]["temperature"]["delta"].toDouble());
}

void Lab::increaseTemperature()
{
    //somme de la temperature actuelle de la boite + le delta
    boite.setTemperature(getTemperature() + getAppConfig()["petri dish"]["temperature"]["delta"].toDouble());
}

double Lab::getTemperature() const
{
    return boite.getTemperature();
}

void Lab :: setTemperatureDefault()
{
    boite.setTemperature(boite.getTemperatureDefault());
}
void Lab:: reset()
{
    boite.reset();
    generator.reset();
}

bool Lab::doesCollideWithDish(CircularBody const& body) const
{
    //si la distance entre les deux centres est sup égale à boite.rayon - body-rayon
    // sup égale et pas seulement égale car ils se deplacent par pas
    if(distance(boite.getPosition(), body.getPosition()) >= (boite.getRadius() - body.getRadius())) {
        return true;
    }
    return false;
}

Nutriment* Lab::getNutrimentColliding(CircularBody const& body) const
{
    return boite.getNutrimentColliding(body);
}

double Lab::getPositionScore(const Vec2d& p) const
{
    return boite.score(p);
}

void Lab::increaseGradientExponent()
{
    boite.setPuissance(boite.getPuissance() + getAppConfig()["petri dish"]["gradient"]["exponent"]["delta"].toDouble());
}
void Lab::decreaseGradientExponent()
{
    boite.setPuissance(boite.getPuissance() - getAppConfig()["petri dish"]["gradient"]["exponent"]["delta"].toDouble());
}

double Lab::getGradientExponent()
{
    return boite.getPuissance();
}

void Lab::setGradientExponentDefault()
{
    boite.setPuissance((getAppConfig()["petri dish"]["gradient"]["exponent"]["max"].toDouble() +
                        getAppConfig()["petri dish"]["gradient"]["exponent"]["min"].toDouble())/2);
}

void Lab::addSwarm(Swarm* b)
{
    boite.AddSwarm(b->getIdentificateur());
}

Swarm* Lab::getSwarmWithId(std::string id) const
{
    return boite.getSwarmWithId(id);
}


double Lab::distance_nutriment(Vec2d pos) const
{
    return boite.distance_nutriment(pos);
}
