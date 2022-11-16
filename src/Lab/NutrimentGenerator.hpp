#pragma once
#include "../Interface/Updatable.hpp"



class NutrimentGenerator : public Updatable
{
public:
    //constructeur
    NutrimentGenerator();

    ~NutrimentGenerator() = default;

    void update(sf::Time dt) override final;

    void reset();

private:

    sf::Time compteur;
};



