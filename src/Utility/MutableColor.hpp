#pragma once

#include "MutableNumber.hpp"
#include "../JSON/JSON.hpp"

#include <array>
#include <SFML/Graphics.hpp>

class MutableColor
{
public:
    //constructeur
    MutableColor(j::Value const& config);

    ~MutableColor() = default;

    //fait muter les MutableNumber du tableau
    void mutate();

    //retourne la couleur
    sf::Color get() const;

private:
    //couleur RGBA (case 0: R; case 1: G; …)
    std::array <MutableNumber, 4> color;
};
