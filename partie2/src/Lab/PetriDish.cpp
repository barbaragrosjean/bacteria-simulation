#include "PetriDish.hpp"
#include "../Utility/Utility.hpp"
#include "../Application.hpp"


// constructeur
PetriDish ::PetriDish (Vec2d position, double rayon)
    : CircularBody(position,rayon)
    , temperature(getTemperatureDefault())
{};

// destructeur doit désalouer la mémoire de bactéries et nutriments
PetriDish :: ~PetriDish()
{
    for(auto bac : bacteries) {
        delete bac;
    };
    for(auto nut : nutriments) {
        delete nut;
    };
};

// peupler l'assiette :true si on a réussit a mettre les bac dans l'assiette
bool PetriDish ::addBacterium(Bacterium* b)
{};

// ajouter des nutriment a l'assiette true : si on a reussit a mettre les nutriments dans l'assiette
//on utilise contains de CircularBody
bool PetriDish ::addNutriment(Nutriment* n)
{
    if(contains(*n)) {
        nutriments.push_back(n);
        return true;
    }
    return false;
};

// sert a faire evoluer les bacteries et nutriments dans le temps
void PetriDish ::update(sf::Time dt)
{
    for(auto n : nutriments) {
        (*n).update(dt);
    }
};

/*supprimer tout les nutriments et les bac d'une assiette
 * éliminer les pointeurs
 * éliminer les éléments du tableau */

void PetriDish ::reset()
{
    for(auto n : nutriments) {
        n=nullptr;
        nutriments.pop_back();
    }
    for(auto b : bacteries) {
        b=nullptr;
        bacteries.pop_back();
    }

    temperature = getTemperatureDefault();

};

// dessiner le contour, les bact et les nutriments
void PetriDish ::drawOn(sf::RenderTarget& targetWindow) const
{
    auto border = buildAnnulus(centre, rayon, sf::Color::Black, 5);
    targetWindow.draw(border);

    for(auto n : nutriments) {
        (*n).drawOn(targetWindow);
    }
}

double PetriDish::getTemperature() const
{
    return temperature;
}

double PetriDish::getTemperatureDefault() const
{
    return getAppConfig()["petri dish"]["temperature"]["default"].toDouble();
}

void PetriDish::setTemperature(double t)
{
    temperature = t;
}


