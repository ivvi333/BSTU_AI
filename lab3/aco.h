#ifndef ACO_H
#define ACO_H

#include <vector>

#include "graph.h"

class ACO
{
public:
    ACO(int alpha, int beta, double rho, double tau0, int numIterations)
        : alpha{alpha}, beta{beta}, rho{rho}
        , tau0{tau0}, numIterations{numIterations}
    {}

    void solve(const Graph *G);

    int getBestLength() const { return bestAnt.pathLength; }

    std::vector<int> getBestPath() const { return bestAnt.path; }

    void setAlpha(int newAlpha);

    void setBeta(int newBeta);

    void setNumIterations(int newNumIterations);

    void setRho(double newRho);

    void setTau0(double newTau0);

private:
    // Структура "Муравей"
    struct Ant {
        Ant(): pathLength{0}, path{std::vector<int>()} {}

        int pathLength;              // общая длина пути
        std::vector<int> path;       // массив вершин пути
    };
    Ant bestAnt;
    // alpha - вес феромона, beta - вес видимости,
    // numIterations - кол-во итераций
    int alpha, beta, numIterations;
    // rho - коэффициент испарения феромона,
    // tau0 - начальное кол-во феромона
    double rho, tau0;
    // tabu - список табу
    std::vector<int> tabu;

    void clearTabu();
    double edgeChoiceProbability(int v1, int v2, std::vector<std::vector<double>> tau
                                , std::vector<std::vector<double>> eta);
    void updatePheromone(std::vector<Ant> ants, std::vector<std::vector<double>> tau);
};

#endif // ACO_H
