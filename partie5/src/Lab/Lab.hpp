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
    void drawOn(sf::RenderTarget& targetWindow) const final;

    // evolue le contenu de la boite
    void update(sf::Time dt) final;

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

    //reinitialise la temperature de la boite à la valeur par défaut du fichier de configuration
    void setTemperatureDefault();

    //reset le lab
    void reset();

    //renvoie true si le body est en collision avec paroi de boite
    bool doesCollideWithDish(CircularBody const& body) const;

    //renvoie le nutriment en collision avec le body
    Nutriment* getNutrimentColliding(CircularBody const& body) const;

    //retourne le score de la position
    double getPositionScore(const Vec2d& position) const;

    //augmente, diminue et get le gradient
    void increaseGradientExponent();
    void decreaseGradientExponent();
    double getGradientExponent() const;

    //reset le gradient à defaut
    void setGradientExponentDefault();

    //ajoute un swarm
    void addSwarm(Swarm* swarm);

    //retourne le swarm avvec cet id
    Swarm* getSwarmWithId(const std::string& id) const;


    //calcul du nouveau graph
    std::unordered_map<std::string, double> fetchData(const std::string & title) const;

    //incremente/decremente nombre bact
    void increaseNumberBacteria(const std::string& type);
    void decreaseNumberBacteria(const std::string& type);


    //true->incremente/false->decremente la somme des parametres mutables
    void changeParametreTwitching(double delta1, const std::string& parametre1, double delta2, const std::string& parametre2, bool up);
    void changeTumbleSimple(double delta1, const std::string& parametre1, double delta2, const std::string& parametre2, bool up);

private:
    // un lab est caractérisé par sa boite de petri
    PetriDish boite;

    // argument qui permet de generer automatiquement des nouveaux nutriments
    NutrimentGenerator generator;

    //quantite totale des nutriments
    Quantity tot_quantity_nutriments() const;

    //numero de soures de nutriments de la boite
    int nr_nutriments() const;

    //get la somme totale du parametre
    double getParametreTwitching(const std::string& parametre) const;
    double getTumbleSimple(const std::string& parametre) const;

    //get le nombre de bacterie de la petridish
    unsigned int getNumberBacteria(const std::string& type) const;
};



