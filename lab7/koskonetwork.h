#ifndef KOSKONETWORK_H
#define KOSKONETWORK_H

#include <vector>

class KoskoNetwork
{
public:
    KoskoNetwork();
    ~KoskoNetwork();

    void train(int neuronCount, std::vector<std::vector<float>> input);

    std::vector<float> classify(std::vector<float> input);

    std::vector<std::vector<float> > getClasses() const;

private:
    float activation(float x);
    std::vector<std::vector<float>> activateMatrix(const std::vector<std::vector<float>>& mat);
    std::vector<std::vector<float>> divideMatrixByNumber(const std::vector<std::vector<float>>& mat, float number);
    std::vector<std::vector<float>> transposeMatrix(const std::vector<std::vector<float>>& mat);
    std::vector<std::vector<float>> addMatrix(const std::vector<std::vector<float>>& mat1,
                                              const std::vector<std::vector<float>>& mat2);
    std::vector<std::vector<float>> multiplyMatrix(const std::vector<std::vector<float>>& mat1,
                                                   const std::vector<std::vector<float>>& mat2);
    bool matrixAreEqual(const std::vector<std::vector<float>>& mat1,
                        const std::vector<std::vector<float>>& mat2);

    std::vector<std::vector<float>> weights;
    std::vector<std::vector<float>> classes;
};

#endif // KOSKONETWORK_H
