#pragma once

#include "PetriDish.hpp"
#include "../Interface/Drawable.hpp"
#include "../Interface/Updatable.hpp"
#include "NutrimentGenerator.hpp"


class Lab : public Drawable, public Updatable
{
public:

    // constructeur du lab : place la boite de petri au centre de la fenêtre
    Lab();

    // destructeur par defaut
    ~Lab() = default;

    //on ne va pas permettre de copie d'un Lab
    Lab(Lab const& other) = delete;
    Lab operator=(Lab const& other) = delete;

    //true si le corps est à l'interieur de l'assiette
    bool contains(const CircularBody& body) const;

    // dessine le lab + ce qu'il contient
    void drawOn(sf::RenderTarget& targetWindow) const ;

    // evolue le contenu de la boite
    void update(sf::Time dt);

    //ajoute un nutriment à la boite
    void addNutriment(Nutriment* nutriment);

    //ajoute des bactérie a la boite
    void addBacterium(Bacterium* bacterium);

    //diminue temp.
    void decreaseTemperature();

    //augmente temp.
    void increaseTemperature();

    //donne la temp. de la boite
    double getTemperature() const;

    //reinitialise la temperature de la boite
    void setTemperatureDefault();

    //reset le lab
    void reset();

    //renvoie true si le body est en collision avec paroi de boite
    bool doesCollideWithDish(CircularBody const& body) const;

    //renvoie le nutriment en collision avec le body
    Nutriment* getNutrimentColliding(CircularBody const& body) const;

    //retourne le score de p
    double getPositionScore(const Vec2d& p) const;

    //augmente, diminue et get le gradient
    void increaseGradientExponent();
    void decreaseGradientExponent();
    double getGradientExponent();

    //reset le gradient à defaut
    void setGradientExponentDefault();

    //ajoute un swarm
    void addSwarm(Swarm* b);

    //retourne le swarm avvec cet id
    Swarm* getSwarmWithId(std::string id) const;

    //renvoie la distance du plus proche nutriment pour ce body
    double distance_nutriment(Vec2d pos) const;

private:
    // un lab est caractérisé par sa boite de petri
    PetriDish boite;

    // argument qui permet de generer automatiquement des nouveaux nutriments
    NutrimentGenerator generator;
};



