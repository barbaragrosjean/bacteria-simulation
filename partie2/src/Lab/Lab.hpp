#pragma once
#include "PetriDish.hpp"

class Lab
{
public:

    // constructeur du lab : place la boite de petri au centre de la fenêtre
    Lab();

    //true si le corps est à l'interieur de l'assiette
    bool contains(const CircularBody& body) const;

    // dessine le lab + ce qu'il contient
    void drawOn(sf::RenderTarget& targetWindow) const ;

    // evolue le contenu de la boite
    void update(sf::Time dt);

    //ajoute un nutriment à la boite
    void addNutriment(Nutriment* nutriment);

    //diminue temp.
    void decreaseTemperature();

    //augmente temp.
    void increaseTemperature();

    //donne la temp. de la boite
    double getTemperature() const;

    //reset le lab
    void reset();

private:
    // un lab est caractérisé par sa boite de petri
    PetriDish boite;
};



