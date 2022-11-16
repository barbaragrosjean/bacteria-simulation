#pragma once

#include "CircularBody.hpp"
#include "../Utility/Types.hpp"
#include "../Interface/Drawable.hpp"
#include "../Interface/Updatable.hpp"

//pre-declaration de bacterium
class Bacterium;
class SimpleBacterium;
class TwitchingBacterium;
class SwarmBacterium;
class FriendlyBacterium;

//la classe hérite de CircularBody
class Nutriment : public CircularBody, public Drawable, public Updatable
{
public:

    //Constructeur initialise le rayon du nutriment avec la même valeur que la quantité
    Nutriment(Quantity quantity, Vec2d center);

    //Destructeur
    virtual ~Nutriment() = default;

    //met à jour la quantité
    void setQuantity(Quantity quantity);

    //retourne la quantité de nutriment qui a pu être prelevée
    Quantity takeQuantity(Quantity quantity);

    //methode de dessin
    virtual void drawOn(sf::RenderTarget& target) const ;

    //calcul l'evvolution du nutriment apres un certain temps dt
    virtual void update(sf::Time dt) ;

    //true si le nutriment est fini (quantite = 0)
    bool estfini() const;

    //score d'un nutriment par rapport à la position
    double score( const Vec2d& position, double puissance) const;

    //differente facon de manger un nutriment (selon le type de la bacterie qui le mange)
    virtual Quantity eatenBy(Bacterium& bact) = 0;
    virtual Quantity eatenBy(SimpleBacterium& bact) = 0;
    virtual Quantity eatenBy(TwitchingBacterium& bact) = 0;
    virtual Quantity eatenBy(SwarmBacterium& bact) = 0;
    virtual Quantity eatenBy(FriendlyBacterium& bact) = 0;

private:
    Quantity quantite;

    //raccourci qui prend en compte direct ["nutriment"]
    virtual j::Value const& getConfig() const =0;

};



