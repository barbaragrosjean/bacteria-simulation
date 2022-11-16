#include "Bacterium.hpp"
#include "../Utility/Utility.hpp"
#include "../Application.hpp"

#include <cmath>

#include "Nutriment.hpp"
#include "NutrimentA.hpp"
#include "NutrimentB.hpp"



//constructeur
Bacterium::Bacterium(Quantity energie, Vec2d position, Vec2d direction, double rayon, MutableColor couleur, bool abst)
    : CircularBody(position, rayon), couleur(couleur)
    , DirectionDeplacement(direction), abstinence(abst), energie(energie)
    , mealClock(sf::Time::Zero)
{
    rotation = DirectionDeplacement.angle();
};

void Bacterium::drawOn(sf::RenderTarget &target) const
{
    //vecteur pour avoir l'écriture de l'energie à coté de la bacterie
    Vec2d deplacement(0, rayon);

    //quand le mode debug est enclenché on peut voir l'energie de la bacterie
    if(isDebugOn()) {
        auto const text = buildText(std::to_string((int) (energie)), centre + deplacement, getAppFont(), 15, sf::Color::Red, 0);
        target.draw(text);
    }

    //dessin du cercle de la bacterie
    auto const circle = buildCircle(centre, rayon, couleur.get());
    target.draw(circle);
};

void Bacterium ::update(sf::Time dt)
{
    mealClock += dt;

    move(dt);

    if(getAppEnv().doesCollideWithDish(*this)) {
        DirectionDeplacement = -DirectionDeplacement;

        changeRotation(dt);
    }

    //pour eviter la duplication de code, nous gardons l'adresse dans cette variable locale
    Nutriment* food(getAppEnv().getNutrimentColliding(*this));

    if((mealClock >= getTempsAttente()) and (!abstinence) and (food != nullptr)) {
        //il mange, donc compteur a zero
        mealClock = sf::Time::Zero;

        eat(*food);
    }
}

//renvoit true si la bacterie est morte
bool Bacterium::estmorte() const
{
    if(energie <= 0.0) {
        return true;
    }
    return false;
};

Quantity Bacterium::getEnergieDivision() const
{
    return getConfig()["energy"]["division"].toDouble();
}

sf::Time Bacterium::getTempsAttente() const
{
    return sf::seconds(getConfig()["meal"]["delay"].toDouble());
}

Quantity Bacterium::getEnergieDepensee() const
{
    return getConfig()["energy"]["consumption factor"].toDouble();
}

Quantity Bacterium::getMaxQuantite() const
{
    return getConfig()["meal"]["max"].toDouble();
}

void Bacterium::consumeEnergy(Quantity qt)
{
    energie -= qt;
}


void Bacterium::addProperty(const std::string& name, MutableNumber number)
{
    //je cherche s pour voir si ça existe
    auto paire = parametre.find(name);

    if(paire != parametre.end()) {
        //lance exception si ça existe pas
        throw std::out_of_range(name);
    } else {
        parametre[name] = number;
    }
}

MutableNumber Bacterium::getProperty(const std::string& name) const
{
    auto paire = parametre.find(name);

    //envoyer erreur si on ne trouvve pas
    if(paire == parametre.end()) {
        throw std::invalid_argument(name);
    } else {
        return paire->second;
    }
}

Bacterium* Bacterium::division()
{
    if (energie >= getEnergieDivision()) {
        energie = energie/2;
        Bacterium* nouvelle = clone();

        // appel des mutations sur nouvelle
        nouvelle->mutate();

        //on inverse la direction de deplacement de la bac
        this->DirectionDeplacement= -this->DirectionDeplacement;
        return nouvelle;
    }

    return nullptr;
}

Vec2d Bacterium:: best_direction() const
{
    struct Position_score {
        Vec2d direction;
        double score;
    };

    //creation du tableau de struct
    std::vector<Position_score> possibility;
    for(size_t i(0); i < 20; ++i) {
        Vec2d direction(Vec2d::fromRandomAngle());
        Position_score nouveau = {direction, getAppEnv().getPositionScore(centre + direction)};
        possibility.push_back(nouveau);
    }

    Position_score best = possibility[0];

    //trouver le meilleur score
    for(size_t i(0); i < possibility.size(); ++i) {
        if(possibility[i].score >= best.score) {
            best = possibility[i];
        }
    }

    return best.direction;
}

void Bacterium::mutate()
{
    for(auto& p: parametre) {
        MutableNumber ancien = p.second;
        p.second.mutate();
        MutableNumber nouveau = p.second;
        if(nouveau.get() == 0) {
            p.second = ancien;
        }
    }
    couleur.mutate();
}


void Bacterium::basculement(const Vec2d &position, const Vec2d &speed)
{
    //déplacement:
    Vec2d deplacement(position - centre);
    if(deplacement.lengthSquared() >= 0.001) {
        centre = position;
    }

    //vitesse et direction de deplacement sont liées
    DirectionDeplacement = speed/speed.length();

    rotation = DirectionDeplacement.angle();
}

void Bacterium::eat(Nutriment& nutriment)
{
    //modifie le nutriment selon ce qui est mangé
    Quantity eaten(nutriment.eatenBy(*this));

    //incremente l'energie selon le repas
    energie += eaten;
}

void Bacterium::changeRotation(sf::Time dt)
{
    auto const angleDiff = angleDelta(DirectionDeplacement.angle(), rotation); // calcule la différence entre le nouveau angle de direction et l'ancien
    auto dalpha = PI * dt.asSeconds(); // calcule dα
    dalpha = std::min(dalpha, std::abs(angleDiff)); // on ne peut tourner plus que de angleDiff
    dalpha = std::copysign(dalpha, angleDiff); // on tourne dans la direction indiquée par angleDiff
    rotation += dalpha; // angle de rotation mis à jour
}







