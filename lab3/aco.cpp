#include "aco.h"

#include <random>
#include <cmath>

void ACO::solve(const Graph *G)
{
    const int numVertices = G->getNumVertices();

    // Очищаем лучшего муравья
    bestAnt.path.clear();
    bestAnt.pathLength = 0;
    for (int i = 0; i < numVertices; i++) {
        bestAnt.path.push_back(i);
        bestAnt.pathLength += G->getA()[i][(i + 1) % numVertices];
    }
    bestAnt.path.push_back(0);

    // Создаём массив муравьёв и список табу
    std::vector<Ant> ants(numVertices);
    tabu = std::vector<int>(numVertices);

    // Создаём матрицы феромонов и обратных расстояний
    std::vector<std::vector<double>> tau, eta;
    for (int i = 0; i < numVertices; i++) {
        tau.push_back(std::vector<double>(numVertices));
        eta.push_back(std::vector<double>(numVertices));
        for (int j = 0; j < numVertices; j++) {
            if (i != j) {
                tau[i][j] = tau0;
                eta[i][j] = 1.0 / G->getA()[i][j];
            }
        }
    }

    // Основной цикл
    for (int t = 0; t < numIterations; t++) {
        // Для каждого муравья
        for (int k = 0; k < numVertices; k++) {
            ants[k].pathLength = 0;
            ants[k].path.clear();
            ants[k].path.push_back(k);
            tabu[k]++;

            // Пока не прошёл все вершины, кроме первой
            while (ants[k].path.size() != numVertices) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<double> testDist;
                double probTest = testDist(gen);
                while (probTest == 0.0) {
                    probTest = testDist(gen);
                }
                // Выбираем следующую вершину
                double probSum = 0.0;
                int nextVertex = 0;
                for (; nextVertex < numVertices && probSum < probTest; nextVertex++) {
                    if (!tabu[nextVertex]) {
                        probSum += edgeChoiceProbability(ants[k].path.back(), nextVertex, tau, eta);
                    }
                }
                nextVertex--;
                ants[k].pathLength += G->getA()[ants[k].path.back()][nextVertex];
                ants[k].path.push_back(nextVertex);
                tabu[nextVertex]++;
            }
            ants[k].pathLength += G->getA()[ants[k].path.back()][k];
            ants[k].path.push_back(k);

            // Если полученный маршрут короче лучшего
            if (ants[k].pathLength < bestAnt.pathLength) {
                bestAnt.pathLength = ants[k].pathLength;
                bestAnt.path = ants[k].path;
            }

            // Очищаем список табу
            clearTabu();
        }

        // Обновляем феромоны
        updatePheromone(ants, tau);
    }
}

void ACO::setAlpha(int newAlpha)
{
    alpha = newAlpha;
}

void ACO::setBeta(int newBeta)
{
    beta = newBeta;
}

void ACO::setNumIterations(int newNumIterations)
{
    numIterations = newNumIterations;
}

void ACO::setRho(double newRho)
{
    rho = newRho;
}

void ACO::setTau0(double newTau0)
{
    tau0 = newTau0;
}

void ACO::clearTabu() {
    for (int i = 0; i < tabu.size(); i++) {
        tabu[i] = 0;
    }
}

double ACO::edgeChoiceProbability(int v1, int v2, std::vector<std::vector<double>> tau
                                 , std::vector<std::vector<double>> eta)
{
    double p = pow(tau[v1][v2], alpha) * pow(eta[v1][v2], beta);
    double sum = 0.0;
    for (int v3 = 0; v3 < tau.size(); v3++) {
        if (!tabu[v3]) {
            sum += pow(tau[v1][v3], alpha) * pow(eta[v1][v3], beta);
        }
    }
    return p / sum;
}

void ACO::updatePheromone(std::vector<Ant> ants, std::vector<std::vector<double>> tau)
{
    const double Q = 1.0;

    for (int i = 0; i < tau.size(); i++) {
        for (int j = 0; j < tau.size(); j++) {
            tau[i][j] *= rho;
        }
    }

    for (const Ant &ant : ants) {
        double delta = Q / ant.pathLength;

        for (int i = 0; i < tau.size(); i++) {
            tau[ant.path[i]][ant.path[i + 1]] += delta;
            tau[ant.path[i + 1]][ant.path[i]] += delta;
        }
    }
}
