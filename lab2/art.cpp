#include "art.h"

void ART::solve()
{
    init();

    std::vector<int> andResult(maxFeatures);
    char count = 50;
    bool done = false;

    while (!done) {
        done = true;

        for (int i = 0; i < maxFeatureVectors; i++) {
            for (int pvec = 0; pvec < maxPrototypeVectors; pvec++) {
                if (members[pvec]) {
                    vectorBitwiseAnd(andResult, featureVectors[i], prototypeVectors[pvec]);
                    int magnitudePE = vectorMagnitude(andResult);
                    int magnitudeE = vectorMagnitude(featureVectors[i]);
                    int magnitudeP = vectorMagnitude(prototypeVectors[pvec]);

                    double result = (double)magnitudePE / (beta + magnitudeP);
                    double test = (double)magnitudeE / (beta + maxFeatures);

                    if (result > test
                    && (double)magnitudePE / magnitudeE > vigilance
                    && membership[i] != pvec)
                    {
                        int oldCluster = membership[i];
                        membership[i] = pvec;

                        if (oldCluster >= 0) {
                            members[oldCluster]--;
                        }
                        members[pvec]++;

                        if (oldCluster >= 0 && oldCluster < maxPrototypeVectors) {
                            updatePrototypeVectors(oldCluster);
                        }
                        updatePrototypeVectors(pvec);

                        done = false;
                        break;
                    }
                    else if (magnitudeE == 0) {
                        membership[i] = -2;
                        break;
                    }
                }
            }

            if (membership[i] == -1) {
                if (members[maxPrototypeVectors - 1] > 0) {
                    continue;
                }
                membership[i] = createNewPrototypeVector(featureVectors[i]);
                done = false;
            }
        }

        if (done && !count--) break;
    }
}

void ART::setMaxFeatureVectors(int newMaxFeatureVectors)
{
    maxFeatureVectors = newMaxFeatureVectors;
    featureVectors.resize(newMaxFeatureVectors, std::vector<int>(maxFeatures));
    membership.resize(newMaxFeatureVectors, -1);
}

void ART::setMaxFeatures(int newMaxFeatures)
{
    maxFeatures = newMaxFeatures;
    for (int i = 0; i < maxFeatureVectors; i++) {
        featureVectors[i].resize(newMaxFeatures);
    }
    for (int i = 0; i < maxPrototypeVectors; i++) {
        prototypeVectors[i].resize(newMaxFeatures);
    }
}

void ART::setMaxPrototypeVectors(int newMaxPrototypeVectors)
{
    maxPrototypeVectors = newMaxPrototypeVectors;
    prototypeVectors.resize(newMaxPrototypeVectors, std::vector<int>(maxFeatures));
    members.resize(newMaxPrototypeVectors);
}

void ART::setBeta(int newBeta)
{
    beta = newBeta;
}

void ART::setVigilance(double newVigilance)
{
    vigilance = newVigilance;
}

void ART::setFeature(int numVector, int numFeature, int value)
{
    if (numVector <= maxFeatureVectors && numFeature <= maxFeatures) {
        featureVectors[numVector][numFeature] = value;
    }
}

std::vector<std::vector<int>> &ART::getPrototypeVectors()
{
    return prototypeVectors;
}

std::vector<std::vector<int>> &ART::getFeatureVectors()
{
    return featureVectors;
}

std::vector<int> ART::getMembership() const
{
    return membership;
}

void ART::init()
{
    for (int i = 0; i < maxPrototypeVectors; i++) {
        for (int j = 0; j < maxFeatures; j++) {
            prototypeVectors[i][j] = 0;
        }
        members[i] = 0;
    }

    std::fill(membership.begin(), membership.end(), -1);
}

int ART::vectorMagnitude(const std::vector<int> &vector)
{
    int magnitude = 0;
    for (int i = 0; i < maxFeatures; i++) {
        if (vector[i] == 1) {
            magnitude++;
        }
    }
    return magnitude;
}

void ART::vectorBitwiseAnd(std::vector<int> &result, const std::vector<int> &vec1, const std::vector<int> &vec2)
{
    for (int i = 0; i < maxFeatures; i++) {
        result[i] = vec1[i] && vec2[i];
    }
}

int ART::createNewPrototypeVector(const std::vector<int> &featureVector)
{
    int cluster;
    for (cluster = 0; cluster < maxPrototypeVectors; cluster++) {
        if (members[cluster] == 0) {
            break;
        }
    }

    for (int j = 0; j < maxFeatures; j++) {
        prototypeVectors[cluster][j] = featureVector[j];
    }

    members[cluster] = 1;

    return cluster;
}

void ART::updatePrototypeVectors(int cluster)
{
    if (cluster < 0) return;

    char first = 1;
    for (int i = 0; i < maxFeatureVectors; i++) {
        if (membership[i] == cluster) {
            if (first) {
                for (int j = 0; j < maxFeatures; j++) {
                    prototypeVectors[cluster][j] = featureVectors[i][j];
                }
                first = 0;
            }

            else {
                for (int j = 0; j < maxFeatures; j++) {
                    prototypeVectors[cluster][j] &= featureVectors[i][j];
                }
            }
        }
    }
}
