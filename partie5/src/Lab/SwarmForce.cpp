#include "SwarmForce.hpp"
//#include "../Lab/Swarm.hpp"
#include "../Application.hpp"


SwarmForce :: SwarmForce(std::string id)
    :identificateur(id)
{};

Vec2d SwarmForce::f(Vec2d position, Vec2d speed) const
{
    //calcule de la force : coeff_force_swarm * (position_leader - position)

    double coeff(getAppConfig()["swarms"][identificateur]["force factor"].toDouble());
    double x_leader(getAppEnv().getSwarmWithId(identificateur)->getPositionLeader().x);
    double y_leader(getAppEnv().getSwarmWithId(identificateur)->getPositionLeader().y);

    Vec2d force(coeff * (x_leader-position.x), coeff * (y_leader-position.y));
    return force;
}



