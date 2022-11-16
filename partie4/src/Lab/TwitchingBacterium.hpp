#pragma once

#include "Bacterium.hpp"
#include "Grip.hpp"

class TwitchingBacterium : public Bacterium
{
public:
    TwitchingBacterium(Vec2d position);

    ~TwitchingBacterium() = default;

    void drawOn(sf::RenderTarget &target) const override;

    //deplacement dépend de l'état de la bacterie
    void move(sf::Time dt) override;

    //méthode de update
    void update(sf::Time dt) override;

    //méthode de clonage
    TwitchingBacterium* clone() const override;


    //getter de l'energie depensée à chaque mouvement
    Quantity getEnergieMouvement() const;

    //getter de l'energie depensée pour le tentacule
    Quantity getEnergieTentacule() const;

    //méthode pour déplacer le grip
    void moveGrip(const Vec2d& delta);

private:
    virtual  j::Value& getConfig() const override;

    //tentacule
    Grip grip;

    //état de la bacterie
    enum etat { IDLE, WAIT_TO_DEPLOY, DEPLOY, ATTRACT, RETRACT, EAT};
    etat act;


};
