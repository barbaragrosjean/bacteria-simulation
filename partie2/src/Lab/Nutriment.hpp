#pragma once

#include "CircularBody.hpp"
#include "../Utility/Types.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

//la classe hérite de CircularBody
class Nutriment : public CircularBody
{
public:

    //Constructeur initialise le rayon du nutriment avec la même valeur que la quantité
    Nutriment(Quantity q, Vec2d c);

    //met à jour la quantité
    void setQuantity (Quantity q);

    //retourne la quantité de nutriment qui a pu être prelevée
    Quantity takeQuantity(Quantity q);

    //methode de dessin
    void drawOn(sf::RenderTarget& target) const;


    //calcul l'evvolution du nutriment apres un certain temps dt
    void update(sf::Time dt);

private:
    Quantity quantite;

    //raccourci qui prend en compte direct ["nutriment"]
    j::Value const& getConfig() const;

};



