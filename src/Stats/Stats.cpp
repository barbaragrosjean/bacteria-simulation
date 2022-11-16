#include <Stats/Stats.hpp>
#include "../Application.hpp"

Stats::Stats()
{
    refreshClock = sf::Time::Zero;
}

void Stats::update(sf::Time dt)
{
    refreshClock += dt;

    if(refreshClock > sf::seconds(getShortConfig().stats_refresh_rate)) {
        for(size_t i(0); i < ensemble.size(); ++i) {

            ensemble.at(i).graph->updateData(refreshClock, getAppEnv().fetchData(ensemble.at(i).title));

        }
        //remettre compteur à 0 apres un refresh
        refreshClock = sf::Time::Zero;
    }
}


std::string Stats::getCurrentTitle() const
{
    //cherche l'id, prend la struct, renvoie le nom
    return ensemble.find(id_actif)->second.title;
}

void Stats::next()
{
    ++id_actif;
    if(id_actif > ensemble.size()-1) {
        id_actif = 0;
    }
}

void Stats::previous()
{
    if(id_actif == 0) {
        id_actif = ensemble.size()-1;
    } else {
        --id_actif;
    }
}

void Stats::drawOn(sf::RenderTarget &target) const
{
    ensemble.at(id_actif).graph->drawOn(target);
}

void Stats::reset()
{
    for(auto& element : ensemble) {
        element.second.graph->reset();
    }
    refreshClock = sf::Time::Zero;
}

void Stats::addGraph(unsigned int identificateur, std::string title,
                     std::vector<std::string> const& series, double min, double max, Vec2d size)
{
    //réinitialiser le pointeur
    ensemble[identificateur].graph.reset(new Graph(series, size, min, max));

    //mettre à jour le libellé
    ensemble[identificateur].title = title;

    //le mettre en identifiant actif
    id_actif = identificateur;

}

void Stats::setActive(int id)
{
    auto paire = ensemble.find(id);

    if(paire != ensemble.end()) {
        id_actif = id;
    }
    /*  } else {
          throw std::invalid_argument("Argument invalide");
      } */

    //tester si il existe ??
}
