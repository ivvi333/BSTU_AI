#ifndef ART_H
#define ART_H

#include <vector>

class ART
{
public:
    ART(int numFeatureVectors, int numFeatures, int numPrototypeVectors, int beta, double vigilance)
        : maxFeatureVectors{numFeatureVectors}, maxFeatures{numFeatures}
        , maxPrototypeVectors{numPrototypeVectors}, beta{beta}, vigilance{vigilance}
        , featureVectors{std::vector<std::vector<int>>(numFeatureVectors, std::vector<int>(numFeatures))}
        , prototypeVectors{std::vector<std::vector<int>>(numPrototypeVectors, std::vector<int>(numFeatures))}
        , members{std::vector<int>(numPrototypeVectors)}, membership{std::vector<int>(numFeatureVectors, -1)}
    {}

    void solve();

    void setMaxFeatureVectors(int newMaxFeatureVectors);

    void setMaxFeatures(int newMaxFeatures);

    void setMaxPrototypeVectors(int newMaxPrototypeVectors);

    void setBeta(int newBeta);

    void setVigilance(double newVigilance);

    void setFeature(int numVector, int numFeature, int value);

    std::vector<std::vector<int>> &getPrototypeVectors();

    std::vector<std::vector<int>> &getFeatureVectors();

    std::vector<int> getMembership() const;

private:
    int maxFeatureVectors, maxFeatures, maxPrototypeVectors, beta;
    double vigilance;
    std::vector<std::vector<int>> featureVectors, prototypeVectors;
    std::vector<int> members, membership;

    void init();
    int vectorMagnitude(const std::vector<int> &vector);
    void vectorBitwiseAnd(std::vector<int> &result, const std::vector<int> &vec1, const std::vector<int> &vec2);
    int createNewPrototypeVector(const std::vector<int> &featureVector);
    void updatePrototypeVectors(int cluster);
};

#endif // ART_H
