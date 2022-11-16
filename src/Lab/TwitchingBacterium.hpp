#pragma once

#include "Bacterium.hpp"
#include "Grip.hpp"

class TwitchingBacterium : public Bacterium
{
public:
    TwitchingBacterium(Vec2d position);

    ~TwitchingBacterium();

    void drawOn(sf::RenderTarget &target) const override final;

    //méthode de update
    void update(sf::Time dt) override final;

    //méthode pour déplacer le grip
    void moveGrip(const Vec2d& delta);

    //differencié les nutriments A et B
    Quantity eatableQuantity(NutrimentA& nutriment) override;
    Quantity eatableQuantity(NutrimentB& nutriment) override;


private:
    virtual  j::Value& getConfig() const override final;

    //tentacule
    Grip grip;

    //état de la bacterie
    enum etat { IDLE, WAIT_TO_DEPLOY, DEPLOY, ATTRACT, RETRACT, EAT};
    etat act;

    //deplacement dépend de l'état de la bacterie
    void move(sf::Time dt) override final;

    //méthode de clonage
    TwitchingBacterium* clone() const override final;

    //getter de l'energie depensée à chaque mouvement
    Quantity getEnergieMouvement() const;

    //getter de l'energie depensée pour le tentacule
    Quantity getEnergieTentacule() const;

};
