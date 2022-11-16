#pragma once

#include "CircularBody.hpp"
#include "Bacterium.hpp"
#include "Nutriment.hpp"
#include "../Interface/Updatable.hpp"
#include "../Interface/Drawable.hpp"
#include "Swarm.hpp"

#include <vector>
//#include <SFML/Graphics.hpp>
//#include <iostream>

class PetriDish : public CircularBody, public Updatable, public Drawable
{
public:
    // constructeur
    PetriDish (Vec2d position, double rayon);

    //on demande a ce qu'on ne puisse pas faire de copie
    PetriDish(PetriDish const& other) = delete;
    PetriDish operator=(PetriDish const& other) = delete;

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

    //retourne le nutriment en collision avvec le body
    Nutriment* getNutrimentColliding(CircularBody const& body) const;

    //calcul score de p
    double score(const Vec2d& p) const;

    //getter et setter de puissance
    double getPuissance() const;
    void setPuissance(double p);

    //ajouter un nouveau groupe vide
    void AddSwarm(std ::string id);

    //controle des swarm avec leur id
    Swarm* getSwarmWithId(std::string id) const;

    //renvoie la distance du plus proche nutriment pour ce body
    double distance_nutriment(Vec2d pos) const;

private:

    std:: vector<Bacterium*> bacteries;
    std:: vector<Nutriment*> nutriments;

    // tableau contennat les bacterie nouvellement produites par clone
    std :: vector<Bacterium*> newbac;

    // tableau de groupes
    std::vector<Swarm*> groupes;

    //temperature de la boite
    double temperature;

    double puissance;
};




