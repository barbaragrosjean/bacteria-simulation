#include "MutableColor.hpp"


//la composante de la couleur se situe entre 0 et 1 (donc a toujours des bornes)
MutableColor::MutableColor(j::Value const& config)
    : color{MutableNumber(config["r"]), MutableNumber(config["g"]),
            MutableNumber(config["b"]), MutableNumber(config["a"])}
{}


void MutableColor::mutate()
{
    for(auto& c : color) {
        c.mutate();
    }
}

sf::Color MutableColor::get() const
{
    return { static_cast<sf::Uint8>(color[0].get() * 255),
             static_cast<sf::Uint8>(color[1].get() * 255),
             static_cast<sf::Uint8>(color[2].get() * 255),
             static_cast<sf::Uint8>(color[3].get() * 255) };
}
