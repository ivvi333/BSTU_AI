#include "ga.h"

#include <random>
#include <algorithm>
#include <limits>

void GA::solve()
{
    population.clear();
    for (int i = 0; i < populationSize; i++) {
        population.push_back(generateRandomRoute());
        calculateRouteFitness(population[i]);
    }

    for (int generation = 0; generation < numGenerations; generation++) {
        std::vector<Route> newPopulation;

        for (int i = 0; i < populationSize; i++) {
            Route parent1 = population[getParentIndex()];
            Route parent2 = population[getParentIndex()];
            Route child = crossover(parent1, parent2);
            mutate(child);
            calculateRouteFitness(child);
            newPopulation.push_back(child);
        }

        population = newPopulation;
    }
}

std::vector<int> GA::getBestPath()
{
    std::vector<int> bestPath = populationBestRoute().path;
    bestPath.push_back(bestPath[0]);
    return bestPath;
}

void GA::setPopulationSize(int newPopulationSize)
{
    populationSize = newPopulationSize;
}

void GA::setNumGenerations(int newNumGenerations)
{
    numGenerations = newNumGenerations;
}

void GA::setMutationRate(double newMutationRate)
{
    mutationRate = newMutationRate;
}

void GA::calculateRouteFitness(Route &route)
{
    int totalLength = 0;
    for (int i = 0; i < graph->getNumVertices() - 1; i++) {
        int vertexIndex1 = route.path[i];
        int vertexIndex2 = route.path[i + 1];
        totalLength += graph->getA()[vertexIndex1][vertexIndex2];
    }

    int lastVertexIndex = route.path[graph->getNumVertices() - 1];
    totalLength += graph->getA()[lastVertexIndex][route.path[0]];
    route.pathLength = totalLength;
}

double GA::parentChoiceProbability(const Route &route)
{
    int parentPathLength = route.pathLength;

    int pathLengthSum = 0;
    for (int i = 0; i < population.size(); i++) {
        pathLengthSum += population[i].pathLength;
    }

    return (double)parentPathLength / pathLengthSum;
}

int GA::getParentIndex()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> testDist;
    double probTest = testDist(gen);
    while (probTest == 0.0) {
        probTest = testDist(gen);
    }

    double probSum = 0.0;
    int parentIndex = 0;
    for (; parentIndex < graph->getNumVertices() && probSum < probTest; parentIndex++) {
        probSum += parentChoiceProbability(population[parentIndex]);
    }
    parentIndex--;

    return parentIndex;
}

GA::Route GA::generateRandomRoute()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    Route r;

    for (int i = 0; i < graph->getNumVertices(); i++) {
        r.path.push_back(i);
    }

    std::shuffle(r.path.begin(), r.path.end(), gen);

    return r;
}

GA::Route GA::populationBestRoute()
{
    int bestLength = std::numeric_limits<int>::max();
    int bestIndex = -1;

    for (int i = 0; i < populationSize; i++) {
        if(population[i].pathLength < bestLength) {
            bestLength = population[i].pathLength;
            bestIndex = i;
        }
    }

    return population[bestIndex];
}

GA::Route GA::crossover(const Route &parent1, const Route &parent2)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> indexDist(0, graph->getNumVertices() - 1);

    Route child;
    int crossIndex = indexDist(gen);
    while (crossIndex == graph->getNumVertices() - 1) {
        crossIndex = indexDist(gen);
    }

    for (int i = 0; i <= crossIndex; i++) {
        child.path.push_back(parent1.path[i]);
    }

    for (int i = 0; i < graph->getNumVertices(); i++) {
        if (std::find(child.path.begin(), child.path.end(), parent2.path[i]) == child.path.end()) {
            child.path.push_back(parent2.path[i]);
        }
    }

    return child;
}

void GA::mutate(Route &route)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> testDist;
    for (int i = 0; i < graph->getNumVertices(); i++) {
        if (testDist(gen) <= mutationRate) {
            std::uniform_int_distribution<int> indexDist(0, graph->getNumVertices() - 1);
            int swapIndex = indexDist(gen);
            std::swap(route.path[i], route.path[swapIndex]);
        }
    }
}
