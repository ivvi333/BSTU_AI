#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <float.h>
#include <QLineSeries>

class Solution
{
public:
    Solution(int queenCount, double maxTemp, double minTemp, double tempReductionRate, int stepCount)
        : queenCount{queenCount}, maxTemp{maxTemp}, minTemp{minTemp}, currTemp{maxTemp}
        , tempReductionRate{tempReductionRate}, stepCount{stepCount}
        , solutionEnergy{DBL_MAX}, queenRows{std::vector<int>(queenCount)}
    {}

    Solution(const Solution& s)
        : queenCount{s.queenCount}, maxTemp{s.maxTemp}, minTemp{s.minTemp}, currTemp{s.currTemp}
        , tempReductionRate{s.tempReductionRate}, stepCount{s.stepCount}
        , solutionEnergy{s.solutionEnergy}, queenRows{s.queenRows}
    {}

    void annealingSolve(QtCharts::QLineSeries *badSolutionLineChart,
                        QtCharts::QLineSeries *bestEnergyLineChart,
                        QtCharts::QLineSeries *currTempLineChart);

    void setQueenCount(int newQueenCount);

    void setStepCount(int newStepCount);

    void setSolutionEnergy(double newSolutionEnergy);

    void setMaxTemp(double newMaxTemp);

    void setMinTemp(double newMinTemp);

    void setTempReductionRate(double newTempReductionRate);

    std::vector<int> getQueenRows() const;

private:
    int queenCount, stepCount;
    double maxTemp, minTemp, currTemp, tempReductionRate, solutionEnergy;
    std::vector<int> queenRows;

    void tweakSolution();
    void initSolution();
    void computeEnergy();
};

#endif // SOLUTION_H
