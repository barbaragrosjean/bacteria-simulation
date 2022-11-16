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
}

void Lab:: addNutriment(Nutriment* nutriment)
{
    boite.addNutriment(nutriment);
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

void Lab:: reset()
{
    boite.reset();
}
