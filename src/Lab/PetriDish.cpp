#include "PetriDish.hpp"
#include "../Utility/Utility.hpp"
#include "../Application.hpp"
#include <cmath>


PetriDish ::PetriDish (Vec2d position, double rayon)
    : CircularBody(position,rayon)
    , temperature(getTemperatureDefault())
    , puissance((getAppConfig()["petri dish"]["gradient"]["exponent"]["max"].toDouble() +
                 getAppConfig()["petri dish"]["gradient"]["exponent"]["min"].toDouble())/2)
{
    numberBacteria[s::SIMPLE_BACTERIA] = 0;
    numberBacteria[s::TWITCHING_BACTERIA] = 0;
    numberBacteria[s::SWARM_BACTERIA] = 0;
    numberBacteria[s::FRIENDLY_BACTERIA] = 0;

    parametresTwitching[s::TENTACLE_LENGTH] = 0;
    parametresTwitching[s::TENTACLE_SPEED] = 0;

    tumbleSimple[s::BETTER] = 0;
    tumbleSimple[s::WORSE] = 0;

    lenghtLife[s::SIMPLE_BACTERIA] = sf::Time::Zero;
    lenghtLife[s::TWITCHING_BACTERIA] = sf::Time::Zero;
    lenghtLife[s::SWARM_BACTERIA] = sf::Time::Zero;
    lenghtLife[s::FRIENDLY_BACTERIA] = sf::Time::Zero;
};

// destructeur doit désalouer la mémoire de bactéries et nutriments et vider les tableaux
PetriDish :: ~PetriDish()
{
    for(auto& bac : bacteries) {
        delete bac;
    };
    for(auto& nut : nutriments) {
        delete nut;
    };
    for(auto& gr :groupes) {
        delete gr;
    };

    for(auto& h: help) {
        delete h;
    }

    help.clear();
    groupes.clear();
    bacteries.clear();
    nutriments.clear();
};

// peupler l'assiette :true si on a réussit a mettre les bac dans l'assiette
bool PetriDish ::addBacterium(Bacterium* bacterium)
{
    if(contains(*bacterium) and bacterium != nullptr) {
        bacteries.push_back(bacterium);
        return true;
    }

    //si il n'est pas contenu on doit le détruire
    delete bacterium;
    bacterium = nullptr;

    return false;
};


/*ajouter des nutriment a l'assiette true : si on a reussit a mettre les nutriments dans l'assiette
on utilise contains de CircularBody*/
bool PetriDish ::addNutriment(Nutriment* nutriment)
{
    if(*this > *nutriment and nutriment != nullptr) {
        nutriments.push_back(nutriment);
        return true;
    }

    //si il n'est pas contenu on doit le détruire
    delete nutriment;
    nutriment = nullptr;

    return false;
};

// sert a faire evoluer les bacteries et nutriments dans le temps
void PetriDish ::update(sf::Time dt)
{
    for(auto& n : nutriments) {
        n->update(dt);
        if(n->estfini()) {
            delete n;
            n=nullptr;
        }
    }
    //supprime tout les nullptr issus de la fin d'un nutriment
    nutriments.erase(std::remove(nutriments.begin(), nutriments.end(), nullptr), nutriments.end());

    for(auto& b : bacteries) {
        b->update(dt);
        if(b->estmorte()) {
            delete b;
            b=nullptr;

        } else {
            //on ajoute les nouvelles bacterie issue du clonnage ou nullptr
            newbac.push_back(b->division());
        }
    }
    //on ajoute les newbac au bacteries de la PetriDish
    append(newbac,bacteries);

    //on vide le tableau newbac
    newbac.clear();

    //on appel update() des groupes de bacteries
    for(auto& groupe : groupes) {
        groupe->update(dt);
    }

    //supprime tout les nullptr
    bacteries.erase(std::remove(bacteries.begin(), bacteries.end(), nullptr), bacteries.end());

    for(auto& h: help) {
        h->update(dt);
        if(h->getLifeClock().asSeconds() > getAppConfig()["help"]["delay"]["disappear"].toDouble()) {
            delete h;
            h = nullptr;
        }
    }
    help.erase(std::remove(help.begin(), help.end(), nullptr), help.end());

};



void PetriDish ::reset()
{
    /*supprimer tout les nutriments et les bac d'une assiette
    * éliminer les pointeurs
    * éliminer les éléments du tableau */

    for(auto& n : nutriments) {
        delete n;
        n=nullptr;
    }
    nutriments.clear();

    for(auto& b : bacteries) {
        delete b;
        b=nullptr;
    }
    bacteries.clear();

    for(auto& h : help) {
        delete h;
        h = nullptr;
    }
    help.clear();

    temperature = getTemperatureDefault();

    parametresTwitching.at(s::TENTACLE_LENGTH) = 0;
    parametresTwitching.at(s::TENTACLE_SPEED) = 0;

    tumbleSimple.at(s::BETTER) = 0;
    tumbleSimple.at(s::WORSE) = 0;

    lenghtLife[s::SIMPLE_BACTERIA] = sf::Time::Zero;
    lenghtLife[s::TWITCHING_BACTERIA] = sf::Time::Zero;
    lenghtLife[s::SWARM_BACTERIA] = sf::Time::Zero;
    lenghtLife[s::FRIENDLY_BACTERIA] = sf::Time::Zero;

};

// dessiner le contour, les bact et les nutriments
void PetriDish ::drawOn(sf::RenderTarget& targetWindow) const
{
    auto border = buildAnnulus(centre, rayon, sf::Color::Black, 5);
    targetWindow.draw(border);

    for(auto& n : nutriments) {
        (*n).drawOn(targetWindow);
    }

    for(auto& b : bacteries) {
        (*b).drawOn(targetWindow);
    }

    for(auto& h: help) {
        h->drawOn(targetWindow);
    }
}

double PetriDish::getTemperature() const
{
    return temperature;
}

double PetriDish::getTemperatureDefault() const
{
    return getAppConfig()["petri dish"]["temperature"]["default"].toDouble();
}

void PetriDish::setTemperature(double temp)
{
    temperature = temp;
}


Nutriment* PetriDish::getNutrimentColliding(CircularBody const& body) const
{
    for(auto& nut: nutriments) {
        if(*nut & body) {
            return nut;
        }
    }
    return nullptr;
}

double PetriDish::score(const Vec2d& position) const
{
    double score(0);

    for(auto& n : nutriments) {
        score += n->score(position, puissance);
    }
    return score;
}

double PetriDish::getPuissance() const
{
    return puissance;
}
void PetriDish::setPuissance(double p)
{
    puissance = p;
}

void PetriDish::addSwarm(std::string id)
{
    groupes.push_back(new Swarm(id));
}

Swarm* PetriDish::getSwarmWithId(std::string id) const
{
    for (auto& gr : groupes) {
        if (gr->getIdentificateur() == id) {
            return gr;
        }
    }
    //s'il en a aucun
    return nullptr;
}

int PetriDish::nr_nutriments() const
{
    return nutriments.size();
}

Quantity PetriDish::tot_quantity_nutriments() const
{
    Quantity qt(0);
    for(auto nut: nutriments) {
        //la quantite du nutriment est proportionnelle au rayon
        qt += nut->getRadius();
    }
    return qt;
}

void PetriDish::increaseNumberBacteria(std::string type)
{
    ++numberBacteria.at(type);
}

void PetriDish::decreaseNumberBacteria(std::string type)
{
    --numberBacteria.at(type);
}

unsigned int PetriDish::getNumberBact(std::string type) const
{
    return numberBacteria.at(type);
}

void PetriDish::changeParametreTwitching(double delta1, std::string parametre1, double delta2, std::string parametre2, bool up)
{
    if(up) {
        //on ajoute la longueur/vitesse delta à la map
        parametresTwitching.at(parametre1) += delta1;
        parametresTwitching.at(parametre2) += delta2;
    } else {
        parametresTwitching.at(parametre1) -= delta1;
        parametresTwitching.at(parametre2) -= delta2;
    }
}

double PetriDish::getParametreTwitching(std::string parametre) const
{
    return parametresTwitching.at(parametre);
}


void PetriDish::changeTumbleSimple(double delta1, std::string parametre1, double delta2, std::string parametre2, bool up)
{
    if(up) {
        tumbleSimple.at(parametre1) += delta1;
        tumbleSimple.at(parametre2) += delta2;
    } else {
        tumbleSimple.at(parametre1) -= delta1;
        tumbleSimple.at(parametre2) -= delta2;
    }
}


double PetriDish::getTumbleSimple(std::string parametre) const
{
    return tumbleSimple.at(parametre);
}

void PetriDish::addHelp(const Vec2d& position)
{
    help.push_back(new Help(position));
}

bool PetriDish::helpCollision(const CircularBody& body) const
{
    for(auto& h : help) {
        if(*h & body) {
            return true;
        }
    }
    return false;
}

unsigned int PetriDish::nr_help() const
{
    return help.size();
}

void PetriDish::changeLenghtLife(sf::Time delta, const std::string& type, bool up)
{
    if(up) {
        lenghtLife.at(type) += delta;
    } else {
        lenghtLife.at(type) -= delta;
    }
}

sf::Time PetriDish::getLenghtLife(std::string type) const
{
    return lenghtLife.at(type);
}
