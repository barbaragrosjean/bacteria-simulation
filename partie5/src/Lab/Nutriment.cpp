#include "Nutriment.hpp"
#include "../Application.hpp"
#include "../Utility/Utility.hpp"
#include <cmath>

#include "Bacterium.hpp"
#include "SwarmBacterium.hpp"
#include "TwitchingBacterium.hpp"
#include "SimpleBacterium.hpp"

Nutriment::Nutriment(Quantity quantity, Vec2d center)
    : CircularBody(center, quantity)
    , quantite(quantity)
{}

void Nutriment::setQuantity (Quantity quantity)
{
    if(quantity <= 0) {
        quantite = 0; //le rayon et la quantite sont proportionnels
        rayon = 0;
    } else {
        quantite = quantity;
        rayon = quantity;
    }
}

Quantity Nutriment::takeQuantity(Quantity quantity)
{
    Quantity qi = quantite;

    //on change la quantité du nutriment (et le rayon) de la classe
    setQuantity(quantite-quantity);

    //si la quantié a prelever est superieure à la quantite du nutriment on retourne tout le nutriment
    if(quantity >= qi) {

        return qi;
    }
    // sinon on retourne juste la bonne quantité
    return quantity;

}

void Nutriment::drawOn(sf::RenderTarget& target) const
{
    //vecteur pour avoir l'écriture de la quantite à coté du nutriment et pas au centre
    Vec2d deplacement(rayon, 0);

    // quand le mode debug est enclenché on peut voir la quantite de nutriment
    if(isDebugOn()) {
        auto const text = buildText(std::to_string((int) (quantite +0.5)), centre + deplacement, getAppFont(), 15, sf::Color::Black, 0);
        target.draw(text);
    }
    //nous devons ajouter +0.5 à la quantité pour pouvoir rejoindre le double de la quantité initiale
    //(sinon ça s'arrete quelques decimales avant)

    //chargement texture
    auto const& texture = getAppTexture(getConfig()["texture"].toString());

    //dessin du nutriment
    auto nutrimentSprite = buildSprite(centre, 6, texture);

    // adapte la taille du Sprite au rayon du nutriment:
    nutrimentSprite.setScale(2 * rayon / texture.getSize().x, 2 * rayon / texture.getSize().y);

    target.draw(nutrimentSprite);
}

void Nutriment::update(sf::Time dt)
{
    /*condition de croissance:
     * 1- la temperature de PetriDish doit être dans le bon range
     * 2- ça peut pas augmenter plus que ["nutriments"]["quantity"]["max"]
     * 3- le nutriment doit rester dans l'assiette */


    double speed(getConfig()["growth"] ["speed"].toDouble());
    auto growth = speed * dt.asSeconds();

    if(getAppEnv().getTemperature() >= getConfig()["growth"]["min temperature"].toDouble()
       and  getAppEnv().getTemperature() <= getConfig()["growth"]["max temperature"].toDouble()
       and (quantite + growth) <= getConfig()["quantity"]["max"].toDouble()*2
       and getAppEnv().contains(*this)) {
        quantite += growth;
        rayon += growth;
    }
}

bool Nutriment::estfini() const
{
    if (quantite <= 0) {
        return true;
    }

    return false;
}

double Nutriment::score(const Vec2d& position, double puissance) const
{
    return quantite / (pow(distance(position, centre), puissance));
}
