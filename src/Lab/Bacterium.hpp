#pragma once

#include <string>

#include "CircularBody.hpp"

#include "../Interface/Drawable.hpp"
#include "../Interface/Updatable.hpp"
#include "../Utility/Utility.hpp"
#include "../Utility/MutableColor.hpp"

//predeclaration des nutriments
class Nutriment;
class NutrimentA;
class NutrimentB;


class Bacterium : public CircularBody, public Drawable, public Updatable
{
public:

    //constructeur
    Bacterium(Quantity energie, Vec2d position, Vec2d direction, double rayon, MutableColor couleur, bool abst = false);

    //Destructeur
    virtual ~Bacterium()=default;

    void drawOn(sf::RenderTarget &target) const override;

    void update(sf::Time dt) override;

    //renvoit true si la bacterie est morte
    bool estmorte() const;

    //getter quantite max qu'elle peut manger
    Quantity getMaxQuantite() const;

    //methode de division de bactérie
    Bacterium* division();

    //differentiation entre nutrimentA et nutrimentB
    virtual Quantity eatableQuantity(NutrimentA& nutriment) = 0;
    virtual Quantity eatableQuantity(NutrimentB& nutriment) = 0;

protected:

    MutableColor couleur;

    Vec2d DirectionDeplacement;

    //angle de rotation
    double rotation;

    Quantity energie;

    //compteur de durée de vie
    sf::Time lenghtLife;

    //deplacement dans le temps = nouvelle position a tout les dt
    virtual void move(sf::Time dt) = 0;

    //méthode pour trouvver la meilleure direction
    Vec2d best_direction() const;

    //getter energie depensee a chaque pas
    Quantity getEnergieDepensee() const;

    //ajouter des parametres mutables
    void addProperty(const std::string& name, MutableNumber number);

    //trouver des parametres mutables numerique
    MutableNumber getProperty(const std::string& name) const;

    //méthode pour déplacer la bacterie et basculer
    void basculement(const Vec2d& position, const Vec2d& speed);

    //decremente l'energie de la bacterie
    void consumeEnergy(Quantity qt);

    //changer l'angle de rotation après une collision
    void changeRotation(sf::Time dt);

    //méthode pour prendre de l'énergie des Help
    void takeHelp();

private:

    std::map<std::string, MutableNumber> parametre;

    bool abstinence;

    //compteur mis a zero apres chaque repas
    sf::Time mealClock;

    //compteur de temps pour les help
    sf::Time useHelp;

    virtual j::Value& getConfig() const = 0;

    //méthode qui gère "l'alimentation" de la bact
    void eat(Nutriment& nutriment);

    //clonage
    virtual Bacterium* clone() const = 0;

    //méthode pour faire muter chaque parametre mutable + la couleur
    void mutate();

    //getter de l'energie minimale à la division
    Quantity getEnergieDivision() const;

    //getter temps attente entre repas
    sf::Time getTempsAttente() const;

    //méthode pour ajouter de l'énergie
    void addEnergy(Quantity qt);
};

