#pragma once
#include "../Interface/Drawable.hpp"
#include <string>

#include "CircularBody.hpp"


#include "../Interface/Updatable.hpp"
#include "../Utility/Utility.hpp"
#include "../Utility/MutableColor.hpp"


class Bacterium : public CircularBody, public Drawable, public Updatable
{
public:

    //constructeur
    Bacterium(Quantity e, Vec2d p, Vec2d d, double r, MutableColor c, bool abst = false);

    //Destructeur
    virtual ~Bacterium()=default;

    void drawOn(sf::RenderTarget &target) const override;

    void update(sf::Time dt) override;

    //deplacement dans le temps = nouvelle position a tout les dt
    virtual void move(sf::Time dt) = 0;

    //clonage
    virtual Bacterium* clone() const = 0;

    //renvoit true si la bacterie est morte
    bool estmorte() const;

    //getter de l'energie minimale à la division
    Quantity getEnergieDivision() const;

    //getter temps attente entre nutriment
    sf::Time getTempsAttente() const;

    //getter energie depensee a chaque pas
    Quantity getEnergieDepensee() const;

    //getter quantite max qu'elle peut manger
    Quantity getQuantite() const;

    //decremente l'energie de la bacterie
    void consumeEnergy(Quantity qt);

    //méthode de mutation
    void mutate();

    //ajouter des parametres mutables
    void addProperty(const std::string& s, MutableNumber n);

    //trouver des parametres mutables numerique
    MutableNumber getProperty(const std::string& s) const;

    //methode de division de bactérie
    Bacterium* division();

    //méthode pour déplacer la bacterie et basculer
    void basculement(Vec2d position, Vec2d speed);


protected:

    std::map<std::string, MutableNumber> parametre;

    MutableColor couleur;

    bool abstinence;

    Vec2d DirectionDeplacement;

    Quantity energie;

    //angle de rotation
    double rotation;

    //méthode pour trouvver la meilleure direction
    Vec2d best_direction() const;

private:
    //compteur mis a zero apres chaque repas
    sf::Time mealClock;

    virtual j::Value& getConfig() const = 0;
};

