#include "PetriDish.hpp"
#include "../Utility/Utility.hpp"
#include "../Application.hpp"


// constructeur
PetriDish ::PetriDish (Vec2d position, double rayon)
    : CircularBody(position,rayon)
    , temperature(getTemperatureDefault())
    , puissance((getAppConfig()["petri dish"]["gradient"]["exponent"]["max"].toDouble() +
                 getAppConfig()["petri dish"]["gradient"]["exponent"]["min"].toDouble())/2)
{};

// destructeur doit désalouer la mémoire de bactéries et nutriments et on vide les tableaux ??? pour swarm ????
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

    groupes.clear();
    bacteries.clear();
    nutriments.clear();
};

// peupler l'assiette :true si on a réussit a mettre les bac dans l'assiette
bool PetriDish ::addBacterium(Bacterium* b)
{
    if(contains(*b)) {
        bacteries.push_back(b);
        return true;
    }
    return false;
};


/*ajouter des nutriment a l'assiette true : si on a reussit a mettre les nutriments dans l'assiette
on utilise contains de CircularBody*/
bool PetriDish ::addNutriment(Nutriment* n)
{
    if(contains(*n)) {
        nutriments.push_back(n);
        return true;
    }
    return false;
};

// sert a faire evoluer les bacteries et nutriments dans le temps
void PetriDish ::update(sf::Time dt)
{
    for(auto& n : nutriments) {
        (*n).update(dt);
        if(n->estfini()) {
            delete n;
            n=nullptr;
        }
    }
    //supprime tout les nullptr
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

    //on appel update() des groupes de bacteries pour actualiser le leader a chaque dt
    for(auto& groupe : groupes) {
        groupe->update(dt);
    }

    //supprime tout les nullptr
    bacteries.erase(std::remove(bacteries.begin(), bacteries.end(), nullptr), bacteries.end());
};

/*supprimer tout les nutriments et les bac d'une assiette
 * éliminer les pointeurs
 * éliminer les éléments du tableau */

void PetriDish ::reset()
{
    for(auto& n : nutriments) {
        n=nullptr;
    }
    nutriments.clear();

    for(auto& b : bacteries) {
        b=nullptr;
    }
    bacteries.clear();

    temperature = getTemperatureDefault();

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
}

double PetriDish::getTemperature() const
{
    return temperature;
}

double PetriDish::getTemperatureDefault() const
{
    return getAppConfig()["petri dish"]["temperature"]["default"].toDouble();
}

void PetriDish::setTemperature(double t)
{
    temperature = t;
}


Nutriment* PetriDish::getNutrimentColliding(CircularBody const& body) const
{
    for(auto& nut: nutriments) {
        if(nut->isColliding(body)) {
            return nut;
        }
    }
    return nullptr;
}

double PetriDish::score(const Vec2d& p) const
{
    double score(0);

    for(auto& n : nutriments) {
        score += n->score(p, puissance);
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

void PetriDish::AddSwarm(std::string id)
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
    return nullptr;
}

double PetriDish::distance_nutriment(Vec2d pos) const
{
    double dist(distance(pos, nutriments[0]->getPosition()) - nutriments[0]->getRadius());

    for(auto n: nutriments) {
        if(distance(pos, n->getPosition()) - n->getRadius() <= dist) {
            dist = distance(pos, n->getPosition()) - n->getRadius();
        }
    }
    return dist;
}
