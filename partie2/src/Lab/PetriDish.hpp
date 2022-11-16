#pragma once

#include "CircularBody.hpp"
#include "Bacterium.hpp"
#include "Nutriment.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

class PetriDish : public CircularBody
{
public:
    // constructeur
    PetriDish (Vec2d position, double rayon);

    // constructeur de copie
    // on demande a ce qu'on ne puisse pas faire de copie
    PetriDish(PetriDish const& other) = delete;

    // destructeur
    ~PetriDish();

    // peupler l'assiette :true si on a réussit a mettre les bact dans l'assiette
    bool addBacterium(Bacterium* b);

    // ajouter des nutriment a l'assiette true : si on a reussit a mettre les nutriments dans l'assiette
    bool addNutriment(Nutriment* n);

    // sert a faire evoluer les bacteries/nutriments dans le temps
    void update(sf::Time dt);

    // supprimer tout les nutriments et les bac d'une assiette
    void reset();

    // dessiner le contour, les bac et les nutriments
    void drawOn(sf::RenderTarget& targetWindow) const ;

    //getter de la temperature
    double getTemperature() const;

    //getter de la temperature par défaut
    double getTemperatureDefault() const;

    //setter de la temperature
    void setTemperature(double t);


private:

    std:: vector<Bacterium*> bacteries;
    std:: vector<Nutriment*> nutriments;

    //temperature de la boite
    double temperature;
};




