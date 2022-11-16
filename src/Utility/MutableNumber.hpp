#pragma once

#include "../JSON/JSON.hpp"


class MutableNumber
{
public:

    //constructeurs
    MutableNumber(double valeur, double prob, double ecart_type, bool borne_inf = false,
                  double borne_min = 0.0, bool borne_sup = false, double borne_max = 0.0);
    MutableNumber(j::Value const& config);
    MutableNumber()=default;

    ~MutableNumber() = default;

    //getter de la valeur
    double get() const;

    //pour modifier la valeur
    void set(double v);

    //modifier aleatoirement le nombre
    void mutate();

    //creation d'un MutableNumber de type probabilité (bornes = 0,1)    (méthode qu'on peut invoquer aussi sans instance)
    static MutableNumber probability(double initialValue, double mutationProbability, double sigma);
    static MutableNumber probability(j::Value const& config);

    //creation d'un MutableNumber avec valeur positive                  (méthode qu'on peut invoquer aussi sans instance)
    static MutableNumber positive(double initialValue, double mutationProbability, double sigma, bool hasMax = false, double max = 0.0);
    static MutableNumber positive(j::Value const& config, bool hasMax = false, double max = 0.0);

private:
    double valeur;

    //probabilité de muter:
    double prob;

    //true si il possede une borne
    bool borne_inf_;
    bool borne_sup_;

    double borne_min_;
    double borne_max_;

    double ecart_type;

    //méthode test pour la valeur et les bornes
    void test_bornes();
};

