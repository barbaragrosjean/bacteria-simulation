#include "Help.hpp"

#include "../Utility/Utility.hpp"

Help::Help(Vec2d position)
    : CircularBody(position, 8)
    , lifeClock(sf::Time::Zero)
{}


void Help::drawOn(sf::RenderTarget &target) const
{
    sf::CircleShape octagon(rayon, 6);

    octagon.setPosition(centre);

    octagon.setFillColor(sf::Color::Yellow);

    target.draw(octagon);
}

void Help::update(sf::Time dt)
{
    lifeClock += dt;
}

sf::Time Help::getLifeClock() const
{
    return lifeClock;
}
