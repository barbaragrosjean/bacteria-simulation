#include "Nutriment.hpp"
#include "../Application.hpp"
#include "../Utility/Utility.hpp"

Nutriment::Nutriment(Quantity q, Vec2d c)
    : CircularBody(c, q)
    , quantite(q)
{}

void Nutriment::setQuantity (Quantity q)
{
    if(q <= 0) {
        quantite = 0; //le rayon et la quantite sont proportionnels
        rayon = 0;
    } else {
        quantite = q;
        rayon = q;
    }
}

Quantity Nutriment::takeQuantity(Quantity q)
{
    Quantity qi = quantite;

    //on change la quantité du nutriment de la classe
    setQuantity(quantite-q);

    //si la quantié a prelever est superieure à la quantite du nutriment on retourne tout le nutriment
    if(q >= quantite) {
        return qi;
    }
    // sinon on retourne juste la bonne quantité
    return q;

}

void Nutriment::drawOn(sf::RenderTarget& target) const
{
    //vecteur pour avoir l'écriture de la quantite à coté du nutriment et pas au centre
    Vec2d deplacement(rayon, 0);

    // quand le mode debug est enclenche on peut voir la quantite de nutriment
    if(isDebugOn()) {
        auto const text = buildText(std::to_string((int) (quantite + 0.5)), centre + deplacement, getAppFont(), 15, sf::Color::Black, 0);
        target.draw(text);
    }
    //nous devons ajouter +0.5 à la quantité pour pouvoir rejoindre 100, sinon ça s'arrete à 99 (en double 99.99970)

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

double Nutriment::score( const Vec2d& p, double puissance) const
{
    return quantite / (pow(distance(p, centre), puissance));
}
