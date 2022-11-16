#pragma once

#include "CircularBody.hpp"
#include "../Utility/Types.hpp"
#include "../Interface/Drawable.hpp"
#include "../Interface/Updatable.hpp"

//la classe hérite de CircularBody
class Nutriment : public CircularBody, public Drawable, public Updatable
{
public:

    //Constructeur initialise le rayon du nutriment avec la même valeur que la quantité
    Nutriment(Quantity q, Vec2d c);

    //Destructeur
    virtual ~Nutriment() = default;

    //met à jour la quantité
    void setQuantity(Quantity q);

    //retourne la quantité de nutriment qui a pu être prelevée
    Quantity takeQuantity(Quantity q);

    //methode de dessin
    virtual void drawOn(sf::RenderTarget& target) const;


    //calcul l'evvolution du nutriment apres un certain temps dt
    virtual void update(sf::Time dt);

    //true si le nutriment est fini
    bool estfini() const;

    //score d'un nutriment par raport à p
    double score( const Vec2d& p, double puissance) const;

private:
    Quantity quantite;

    //raccourci qui prend en compte direct ["nutriment"]
    virtual j::Value const& getConfig() const =0;

};



