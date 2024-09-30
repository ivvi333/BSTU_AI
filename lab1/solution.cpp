#include "solution.h"

#include <random>
#include <cmath>

void Solution::annealingSolve(QtCharts::QLineSeries *badSolutionLineChart,
                              QtCharts::QLineSeries *bestEnergyLineChart,
                              QtCharts::QLineSeries *currTempLineChart)
{
    initSolution();
    computeEnergy();
    currTemp = maxTemp;
    Solution working(*this), best(*this);

    int timer = 0;

    while (currTemp > minTemp) {
        int accepted = 0;

        for (int step = 0; step < stepCount; step++) {
            bool useNew{false};
            working.tweakSolution();
            working.computeEnergy();
            if (working.solutionEnergy <= solutionEnergy)
                useNew = true;
            else {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<double> testDist(0.0, 1.0);
                double test{testDist(gen)};

                double delta{working.solutionEnergy - solutionEnergy};
                double calc{exp(-delta / currTemp)};
                if (calc > test) {
                    useNew = true;
                    accepted++;
                }
            }

            if (useNew) {
                useNew = false;
                *this = working;
                if (solutionEnergy < best.solutionEnergy)
                    best = *this;
            }
            else {
                working = *this;
            }
        }

        timer++;
        badSolutionLineChart->append(timer, accepted);
        bestEnergyLineChart->append(timer, best.solutionEnergy);
        currTempLineChart->append(timer, currTemp);
        currTemp *= tempReductionRate;
    }

    *this = best;
}

void Solution::setQueenCount(int newQueenCount)
{
    queenCount = newQueenCount;
    queenRows.resize(newQueenCount);
}

void Solution::setStepCount(int newStepCount)
{
    stepCount = newStepCount;
}

void Solution::setMaxTemp(double newMaxTemp)
{
    maxTemp = newMaxTemp;
}

void Solution::setMinTemp(double newMinTemp)
{
    minTemp = newMinTemp;
}

void Solution::setTempReductionRate(double newTempReductionRate)
{
    tempReductionRate = newTempReductionRate;
}

std::vector<int> Solution::getQueenRows() const
{
    return queenRows;
}

void Solution::tweakSolution()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> queenDist(0, queenCount - 1);

    int x{queenDist(gen)}, y{queenDist(gen)};
    while (x == y) {
        y = queenDist(gen);
    }

    int temp{queenRows[x]};
    queenRows[x] = queenRows[y];
    queenRows[y] = temp;
}

void Solution::initSolution()
{
    for (int i = 0; i < queenCount; i++) {
        queenRows[i] = i;
    }
    for (int i = 0; i < queenCount; i++) {
        tweakSolution();
    }
}

void Solution::computeEnergy()
{
    int conflicts{};

    for (int i = 0; i < queenCount; i++) {
        int x{i};
        int y{queenRows[i]};

        while (x > 0 && y > 0) {
            x--; y--;
        }
        for (; x < queenCount && y < queenCount; x++, y++) {
            if (x != i && queenRows[x] == y)
                conflicts++;
        }

        x = i;
        y = queenRows[i];

        while ((x < (queenCount - 1)) && y > 0) {
            x++; y--;
        }
        for (; x >= 0 && y < queenCount; x--, y++) {
            if (x != i && queenRows[x] == y)
                conflicts++;
        }
    }

    solutionEnergy = conflicts;
}
