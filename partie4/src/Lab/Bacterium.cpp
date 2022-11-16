#include "Bacterium.hpp"
#include "../Utility/Utility.hpp"
#include "../Application.hpp"



//constructeur
Bacterium::Bacterium(Quantity e, Vec2d p, Vec2d d, double r, MutableColor c, bool abst)
    : CircularBody(p, r), couleur(c), abstinence(abst)
    , DirectionDeplacement(d), energie(e)
    , mealClock(sf::Time::Zero)
{
    rotation = DirectionDeplacement.angle();
};

void Bacterium::drawOn(sf::RenderTarget &target) const
{
    //vecteur pour avoir l'écriture de l'energie à coté de la bacterie et pas au centre
    Vec2d deplacement(0, rayon);

    // quand le mode debug est enclenche on peut voir l'energie de la bacterie
    if(isDebugOn()) {
        auto const text = buildText(std::to_string((int) (energie)), centre + deplacement, getAppFont(), 15, sf::Color::Red, 0);
        target.draw(text);
    }

    auto const circle = buildCircle(centre, rayon, couleur.get());
    target.draw(circle);
};

void Bacterium ::update(sf::Time dt)
{
    mealClock += dt;

    move(dt);

    if(getAppEnv().doesCollideWithDish(*this)) {
        DirectionDeplacement = -DirectionDeplacement;

        auto const angleDiff = angleDelta(DirectionDeplacement.angle(), rotation); // calcule la différence entre le nouveau angle de direction et l'ancien
        auto dalpha = PI * dt.asSeconds(); // calcule dα
        dalpha = std::min(dalpha, std::abs(angleDiff)); // on ne peut tourner plus que de angleDiff
        dalpha = std::copysign(dalpha, angleDiff); // on tourne dans la direction indiquée par angleDiff
        rotation += dalpha; // angle de rotation mis à jour
    }

    //pour eviter la duplication de code, nous gardons l'adresse dans cette variable locale
    Nutriment* food(getAppEnv().getNutrimentColliding(*this));

    if((mealClock >= getTempsAttente()) and (!abstinence) and (food != nullptr)) {
        //il mange, donc compteur a zero
        mealClock = sf::Time::Zero;

        //modifie la quantite et rayon du nutriment mangé
        Quantity a_pu_prendre(food->takeQuantity(getQuantite()));


        //adapte l'energie de la bacterie selon ce qu'elle a mangé
        energie += a_pu_prendre;
    }
}

//renvoit true si la bacterie est mortes
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

Quantity Bacterium::getQuantite() const
{
    return getConfig()["meal"]["max"].toDouble();
}

void Bacterium::consumeEnergy(Quantity qt)
{
    energie -= qt;
}


void Bacterium::addProperty(const std::string& s, MutableNumber n)
{
    //je cherche s pour voir si ça existe

    auto paire = parametre.find(s);

    if(paire != parametre.end()) {
        throw std::out_of_range(s);
    } else {
        parametre[s] = n;
    }
}

MutableNumber Bacterium::getProperty(const std::string& s) const
{
    auto paire = parametre.find(s);

    //envoyer erreur si on ne trouvve pas
    if(paire == parametre.end()) {
        throw std::invalid_argument(s);
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
        p.second.mutate();
    }
    couleur.mutate();
}


void Bacterium::basculement(Vec2d position, Vec2d speed)
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
