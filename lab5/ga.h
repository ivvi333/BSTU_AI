#ifndef GA_H
#define GA_H

#include <vector>

#include "graph.h"

class GA
{
public:
    GA(const Graph *graph, const int &populationSize, const double &mutationRate, const int &numGenerations)
        : graph{graph}, populationSize{populationSize}
        , mutationRate{mutationRate}, numGenerations{numGenerations}
    {}

    void solve();

    int getBestLength() { return populationBestRoute().pathLength; }

    std::vector<int> getBestPath();

    void setPopulationSize(int newPopulationSize);

    void setNumGenerations(int newNumGenerations);

    void setMutationRate(double newMutationRate);

private:
    struct Route {
        Route(): pathLength{-1}, path{} {}

        int pathLength;         // Для задачи коммивояжёра приспособленность = длина маршрута (минимальная)
        std::vector<int> path;
    };

    const Graph *graph;
    int populationSize, numGenerations;
    double mutationRate;

    std::vector<Route> population;

    void calculateRouteFitness(Route& route);
    double parentChoiceProbability(const Route& route);
    int getParentIndex();
    Route generateRandomRoute();
    Route populationBestRoute();
    Route crossover(const Route &parent1, const Route &parent2);
    void mutate(Route &route);
};

#endif // GA_H
