#pragma once

#include "CircularBody.hpp"
#include "../Interface/Drawable.hpp"
#include"../Interface/Updatable.hpp"

class Help : public CircularBody, public Updatable, public Drawable
{
public:
    Help(Vec2d position);

    ~Help() = default;

    void drawOn(sf::RenderTarget &target) const override final;

    void update(sf::Time dt) override final;

    sf::Time getLifeClock() const;

private:
    sf::Time lifeClock;
};
