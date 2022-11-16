#pragma once

#include "../Utility/DiffEqSolver.hpp"

//pré-declaration du Swarm
//class Swarm;

class SwarmForce : public DiffEqFunction
{
public:

    SwarmForce(std::string id);

    Vec2d f(Vec2d position, Vec2d speed) const override;

private:
    std::string identificateur;
};

