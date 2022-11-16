#include "NullForce.hpp"

Vec2d NullForce::f(Vec2d position, Vec2d speed) const
{
    //la force est un vecteur bidimensionel nul independammet de la position ou de la vitesse
    return Vec2d(0,0);
}
