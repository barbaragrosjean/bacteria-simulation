#pragma once

#include "CircularBody.hpp"
#include "Bacterium.hpp"
#include "Nutriment.hpp"
#include "../Interface/Updatable.hpp"
#include "../Interface/Drawable.hpp"
#include "Swarm.hpp"
#include "Help.hpp"

#include <vector>


class PetriDish : public CircularBody, public Updatable, public Drawable
{
public:
    // constructeur
    PetriDish (Vec2d position, double rayon);

    //on demande a ce qu'on ne puisse pas faire de copie
    PetriDish(PetriDish const& other) = delete;
    PetriDish operator=(PetriDish const& other) = delete;

    // destructeur
    ~PetriDish();

    // peupler l'assiette :true si on a réussit a mettre les bact dans l'assiette
    bool addBacterium(Bacterium* bacterium);

    // ajouter des nutriment a l'assiette true : si on a reussit a mettre les nutriments dans l'assiette
    bool addNutriment(Nutriment* nutriment);

    // sert a faire evoluer les bacteries + nutriments dans le temps
    void update(sf::Time dt) final;

    // supprimer tout les nutriments et les bac d'une assiette
    void reset();

    // dessiner le contour, les bac et les nutriments
    void drawOn(sf::RenderTarget& targetWindow) const final;

    //getter de la temperature
    double getTemperature() const;

    //getter de la temperature par défaut dans le fichier de configuration
    double getTemperatureDefault() const;

    //setter de la temperature
    void setTemperature(double temp);

    //retourne le nutriment en collision avvec le body
    Nutriment* getNutrimentColliding(CircularBody const& body) const;

    //calcul score de p
    double score(const Vec2d& position) const;

    //getter et setter de puissance
    double getPuissance() const;
    void setPuissance(double p);

    //ajouter un nouveau groupe vide
    void addSwarm(std ::string id);

    //revoie le swarm avec cet identificateur
    Swarm* getSwarmWithId(std::string id) const;

    //renvoie le nombre de nutriments
    int nr_nutriments() const;

    //renvoie la quantite totale de nutriments
    Quantity tot_quantity_nutriments() const;

    //incremente/decremente nombre bact du type
    void increaseNumberBacteria(std::string type);
    void decreaseNumberBacteria(std::string type);

    //getter du nombre de bact
    unsigned int getNumberBact(std::string type) const;

    //true -> incremente la somme des parametres mutables, false -> decremente
    void changeParametreTwitching(double delta1, std::string parametre1, double delta2, std::string parametre2, bool up);

    //get la somme totale du parametre twitching
    double getParametreTwitching(std::string parametre) const;

    //true -> incremente la somme du tumble, false -> decremente
    void changeTumbleSimple(double delta1, std::string parametre1, double delta2, std::string parametre2, bool up);

    //get la somme du tumble
    double getTumbleSimple(std::string parametre) const;

    //ajout de Help à une position
    void addHelp(const Vec2d& position);

    //méthode si un body est en collision avec les help
    bool helpCollision(const CircularBody& body) const;

    //méthode pour savoir le nombre de help
    unsigned int nr_help() const;

    //true -> incremente la durée de vie
    void changeLenghtLife(sf::Time delta, const std::string& type, bool up);

    //get le lifespan
    sf::Time getLenghtLife(std::string type) const;

private:

    std:: vector<Bacterium*> bacteries;
    std:: vector<Nutriment*> nutriments;

    // tableau contennat les bacterie nouvellement produites par clone
    std :: vector<Bacterium*> newbac;

    // tableau de groupes
    std::vector<Swarm*> groupes;

    //temperature de la boite
    double temperature;

    double puissance;

    //tableau des aides
    std::vector < Help* > help;

    //map avec nombre de chaque bacterie
    std::map <std::string, unsigned int> numberBacteria;

    //map avec la somme des parametres mutable des TwitchingBacterium
    std::map < std::string, double > parametresTwitching;

    //map avec les tumble des SimpleBacterium
    std::map < std::string, double > tumbleSimple;

    //map avec la durée de vie de chaque type de bactérie
    std::map < std::string, sf::Time > lenghtLife;

};




