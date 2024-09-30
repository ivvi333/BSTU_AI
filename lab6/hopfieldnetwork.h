#ifndef HOPFIELDNETWORK_H
#define HOPFIELDNETWORK_H

#include <vector>

class HopfieldNetwork
{
public:
    HopfieldNetwork(int neuronCount);
    ~HopfieldNetwork();

    void train(std::vector<std::vector<float>> input);

    std::vector<float> classify(std::vector<float> input);

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
};

#endif // HOPFIELDNETWORK_H
