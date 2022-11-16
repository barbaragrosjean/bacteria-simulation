#include "Lab.hpp"
#include "Application.hpp"

Lab::Lab()
    : boite(getApp().getCentre(), ((getApp().getLabSize().x)*0.95)/2)
{}

//on teste avec la methode contains de CircularBody si le body est contenu dans la PetriDish
bool Lab::contains(const CircularBody& body) const
{
    return (boite > body);
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
    // sup égale et pas seulement égale car ils se deplacent par pas et pourrait depasser avec un seul pas
    if(distance(boite.getPosition(), body.getPosition()) >= (boite.getRadius() - body.getRadius())) {
        return true;
    }
    return false;
}

Nutriment* Lab::getNutrimentColliding(CircularBody const& body) const
{
    return boite.getNutrimentColliding(body);
}

double Lab::getPositionScore(const Vec2d& position) const
{
    return boite.score(position);
}

void Lab::increaseGradientExponent()
{
    boite.setPuissance(boite.getPuissance() + getAppConfig()["petri dish"]["gradient"]["exponent"]["delta"].toDouble());
}
void Lab::decreaseGradientExponent()
{
    boite.setPuissance(boite.getPuissance() - getAppConfig()["petri dish"]["gradient"]["exponent"]["delta"].toDouble());
}

double Lab::getGradientExponent() const
{
    return boite.getPuissance();
}

void Lab::setGradientExponentDefault()
{
    boite.setPuissance((getAppConfig()["petri dish"]["gradient"]["exponent"]["max"].toDouble() +
                        getAppConfig()["petri dish"]["gradient"]["exponent"]["min"].toDouble())/2);
}

void Lab::addSwarm(Swarm* swarm)
{
    boite.addSwarm(swarm->getIdentificateur());
}

Swarm* Lab::getSwarmWithId(const std::string &id) const
{
    return boite.getSwarmWithId(id);
}

int Lab::nr_nutriments() const
{
    return boite.nr_nutriments();
}

Quantity Lab::tot_quantity_nutriments() const
{
    return boite.tot_quantity_nutriments();
}

std::unordered_map<std::string, double> Lab::fetchData(const std::string & title) const
{

    if(title == s::NUTRIMENT_QUANTITY) {

        //calculer le nombre de nutriments
        return {{s::NUTRIMENT_QUANTITY, tot_quantity_nutriments()}};
    } else if(title == s::GENERAL) {

        return {{ s::SIMPLE_BACTERIA, getNumberBacteria(s::SIMPLE_BACTERIA)},
            { s::TWITCHING_BACTERIA, getNumberBacteria(s::TWITCHING_BACTERIA)},
            {s::SWARM_BACTERIA, getNumberBacteria(s::SWARM_BACTERIA)},
            {s::FRIENDLY_BACTERIA, getNumberBacteria(s::FRIENDLY_BACTERIA)},
            {s::NUTRIMENT_SOURCES, nr_nutriments()}, {s::DISH_TEMPERATURE, getTemperature()}   };

    } else if(title == s::TWITCHING_BACTERIA) {
        if(boite.getNumberBact(s::TWITCHING_BACTERIA) != 0) {
            return {{s::TENTACLE_LENGTH, getParametreTwitching(s::TENTACLE_LENGTH)/getNumberBacteria(s::TWITCHING_BACTERIA)},
                {s::TENTACLE_SPEED, getParametreTwitching(s::TENTACLE_SPEED)/getNumberBacteria(s::TWITCHING_BACTERIA)}};
        } else {
            return {{s::TENTACLE_LENGTH, 0},
                {s::TENTACLE_SPEED, 0}};
        }
    } else if(title == s::SIMPLE_BACTERIA) {
        if(boite.getNumberBact(s::SIMPLE_BACTERIA) != 0) {
            return {{s::BETTER, getTumbleSimple(s::BETTER)/getNumberBacteria(s::SIMPLE_BACTERIA)},
                {s::WORSE, getTumbleSimple(s::WORSE)/getNumberBacteria(s::SIMPLE_BACTERIA)}};
        } else {
            return {{s::BETTER, 0},
                {s::WORSE, 0}};
        }
    } else if(title == s::HELP) {
        return {{s::HELP, nr_help()}};
    } else if(title == s::LIFESPAN) {
        std::unordered_map<std::string, double> data;
        if(getNumberBacteria(s::SIMPLE_BACTERIA) != 0) {
            data[s::SIMPLE_BACTERIA] = getLenghtLife(s::SIMPLE_BACTERIA).asSeconds()/getNumberBacteria(s::SIMPLE_BACTERIA);
        } else {
            data[s::SIMPLE_BACTERIA] = 0;
        }
        if(getNumberBacteria(s::TWITCHING_BACTERIA) != 0) {
            data[s::TWITCHING_BACTERIA] = getLenghtLife(s::TWITCHING_BACTERIA).asSeconds()/getNumberBacteria(s::TWITCHING_BACTERIA);
        } else {
            data[s::TWITCHING_BACTERIA] = 0;
        }
        if(getNumberBacteria(s::SWARM_BACTERIA) != 0) {
            data[s::SWARM_BACTERIA] = getLenghtLife(s::SWARM_BACTERIA).asSeconds()/getNumberBacteria(s::SWARM_BACTERIA);
        } else {
            data[s::SWARM_BACTERIA] = 0;
        }
        if(getNumberBacteria(s::FRIENDLY_BACTERIA) != 0) {
            data[s::FRIENDLY_BACTERIA] = getLenghtLife(s::FRIENDLY_BACTERIA).asSeconds()/getNumberBacteria(s::FRIENDLY_BACTERIA);
        } else {
            data[s::FRIENDLY_BACTERIA] = 0;
        }
        return data;
        /*{{s::SIMPLE_BACTERIA, getLenghtLife(s::SIMPLE_BACTERIA).asSeconds()/getNumberBacteria(s::SIMPLE_BACTERIA)},
            {s::TWITCHING_BACTERIA, getLenghtLife(s::TWITCHING_BACTERIA).asSeconds()/getNumberBacteria(s::TWITCHING_BACTERIA)},
            {s::SWARM_BACTERIA, getLenghtLife(s::SWARM_BACTERIA).asSeconds()/getNumberBacteria(s::SWARM_BACTERIA)},
            {s::FRIENDLY_BACTERIA, getLenghtLife(s::FRIENDLY_BACTERIA).asSeconds()/getNumberBacteria(s::FRIENDLY_BACTERIA)}};*/
    }

    return {};
}

void Lab::increaseNumberBacteria(const std::string& type)
{
    boite.increaseNumberBacteria(type);
}

void Lab::decreaseNumberBacteria(const std::string &type)
{
    boite.decreaseNumberBacteria(type);
}

unsigned int Lab::getNumberBacteria(const std::string& type) const
{
    return boite.getNumberBact(type);
}

void Lab::changeParametreTwitching(double delta1, const std::string &parametre1, double delta2, const std::string &parametre2, bool up)
{
    boite.changeParametreTwitching(delta1, parametre1, delta2, parametre2, up);
}

void Lab::changeTumbleSimple(double delta1, const std::string &parametre1, double delta2, const std::string &parametre2, bool up)
{
    boite.changeTumbleSimple(delta1, parametre1, delta2, parametre2, up);
}

double Lab::getParametreTwitching(const std::string& parametre) const
{
    return boite.getParametreTwitching(parametre);
}

double Lab::getTumbleSimple(const std::string &parametre) const
{
    return boite.getTumbleSimple(parametre);
}

void Lab::addHelp(const Vec2d& position)
{
    boite.addHelp(position);
}

bool Lab::helpCollision(const CircularBody& body) const
{
    return boite.helpCollision(body);
}

unsigned int Lab::nr_help() const
{
    return boite.nr_help();
}

void Lab::changeLenghtLife(sf::Time delta, const std::string& type, bool up)
{
    boite.changeLenghtLife(delta, type, up);
}

sf::Time Lab::getLenghtLife(std::string type) const
{
    return boite.getLenghtLife(type);
}
