#pragma once
#include "../Utility/DiffEqSolver.hpp"

class NullForce : public DiffEqFunction
{
public:
    Vec2d f(Vec2d position, Vec2d speed) const override;

private:

};
