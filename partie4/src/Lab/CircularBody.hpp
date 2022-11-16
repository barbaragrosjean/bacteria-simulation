#pragma once

#include <iostream>
#include "../Utility/Vec2d.hpp"

class CircularBody
{
public:
    //Contructeur pour rayon et centre
    CircularBody(Vec2d centre, double rayon);

    // Destructeur par defaut
    ~CircularBody() = default;

    //Getter
    const Vec2d& getPosition() const;
    double getRadius() const;

    //Setters
    void setPosition(const Vec2d& c);
    void setRadius(double r);

    //Constructeur de copie
    CircularBody(CircularBody const& other);

    //Operateurs
    void operator=(const CircularBody& source);
    bool operator>(const CircularBody& other) const; //body contenu? oui -> true
    bool operator&(const CircularBody& other) const; //collision? oui -> true
    bool operator>(const Vec2d& point) const; //point contenu? oui -> true

    //pour deplacer le centre
    void move(const Vec2d& deplacement);

    //true si other/point est completement à l'interieur de l'instance courante
    bool contains(const CircularBody& other) const;
    bool contains(const Vec2d& point) const;

    //true si other est en collision
    bool isColliding(const CircularBody& other) const;

protected:
    Vec2d centre;
    double rayon;

};

std::ostream& operator<<(std::ostream& cout, const CircularBody& body);


