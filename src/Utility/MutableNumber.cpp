#include "MutableNumber.hpp"
#include "../Random/Random.hpp"

MutableNumber::MutableNumber(double valeur, double prob, double ecart_type, bool borne_inf,
                             double borne_min, bool borne_sup, double borne_max)
    : valeur(valeur)
    , prob(prob)
    , borne_inf_(borne_inf)
    , borne_sup_(borne_sup)
    , borne_min_(borne_min)
    , borne_max_(borne_max)
    , ecart_type(ecart_type)
{
    test_bornes();
}

MutableNumber::MutableNumber(j::Value const& config)
    : valeur(config["initial"].toDouble())
    , prob(config["rate"].toDouble())
    , borne_inf_(config["clamp min"].toBool())
    , borne_sup_(config["clamp max"].toBool())
    , borne_min_(config["min"].toDouble())
    , borne_max_(config["max"].toDouble())
    , ecart_type(config["sigma"].toDouble())

{
    test_bornes();
}

double MutableNumber::get() const
{
    return valeur;
}

void MutableNumber::set(double v)
{
    valeur = v;
    test_bornes();
}

void MutableNumber::mutate()
{
    if(bernoulli(prob)) {
        valeur += normal(0, ecart_type*ecart_type);
    }
    test_bornes();
}

MutableNumber MutableNumber:: probability(double initialValue, double mutationProbability, double sigma)
{
    return MutableNumber(initialValue, mutationProbability, sigma, true, 0, true, 1);
}

MutableNumber MutableNumber:: probability(j::Value const& config)
{
    //utilisons la méthode codée precedamment
    return probability(config["initial"].toDouble(), config["rate"].toDouble(), config["sigma"].toDouble());
}

MutableNumber MutableNumber::positive(double initialValue, double mutationProbability, double sigma, bool hasMax, double max)
{
    return MutableNumber(initialValue, mutationProbability, sigma, true, 0, hasMax, max);
}

MutableNumber MutableNumber:: positive(j::Value const& config, bool hasMax, double max)
{
    //utilisons la méthode codée precedamment
    return positive(config["initial"].toDouble(), config["rate"].toDouble(), config["sigma"].toDouble(), hasMax, max);
}

void MutableNumber:: test_bornes()
{
    //test pour voir si il y a des bornes et si oui si la valeur y est comprise
    if(borne_inf_ and valeur < borne_min_) {

        //si la valeur est en dehors des bornes on lui affecte la borne
        valeur = borne_min_;
    }
    if(borne_sup_ and valeur > borne_max_) {
        valeur = borne_max_;
    }
}
