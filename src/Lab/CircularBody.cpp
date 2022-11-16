#include "CircularBody.hpp"

CircularBody::CircularBody(Vec2d centre, double rayon)
    : centre(centre)
    , rayon(rayon)
{};

const Vec2d& CircularBody::getPosition() const
{
    return centre;
};

double CircularBody::getRadius() const
{
    return rayon;
};

void CircularBody::setPosition(const Vec2d& position)
{
    centre = position;
};

void CircularBody::setRadius(double radius)
{
    rayon = radius;
};

CircularBody::CircularBody(CircularBody const& other)
    : centre(other.centre)
    , rayon(other.rayon)
{};

void CircularBody::operator=(const CircularBody& source)
{
    rayon = source.rayon;
    centre = source.centre;
};

bool CircularBody::operator>(const CircularBody& other) const
{
    return contains(other);
};

bool CircularBody::operator&(const CircularBody& other) const
{
    return isColliding(other);
};

bool CircularBody::operator>(const Vec2d& point) const
{
    return contains(point);
};

void CircularBody::move(const Vec2d& deplacement)
{
    centre += deplacement;
};


bool CircularBody::isColliding(const CircularBody& other) const
{
    //ils se touchent (true) si la distance entre les centres est <= à la somme des rayons
    if(distance(centre, other.centre) <= (rayon + other.rayon)) {
        return true;
    }
    return false;
};

bool CircularBody::contains(const CircularBody& other) const
{
    /*true si other est dedans:
     * 1) other doit être plus petit
     * 2) la distance entre les centres doit être inferieure à la difference entre les rayons */

    if((rayon >= other.rayon) and (distance(centre, other.centre) <= (rayon - other.rayon))) {
        return true;
    }
    return false;
};

bool CircularBody::contains(const Vec2d& point) const
{
    //true si la distance entre centre et point est <= au rayon
    if(distance(centre, point) <= rayon) {
        return true;
    }
    return false;
};

//operator pour afficher <<
std::ostream& operator<<(std::ostream& cout, const CircularBody& body)
{
    std::cout << "CircularBody: position = " << body.getPosition()
              << " radius = " << body.getRadius() << std::endl;
    return cout;
};

