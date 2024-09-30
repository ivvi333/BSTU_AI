#include "koskonetwork.h"

#include <stdexcept>

KoskoNetwork::KoskoNetwork()
{}

KoskoNetwork::~KoskoNetwork()
{}


float KoskoNetwork::activation(float x)
{
    return (x >= 0 ? 1.0 : -1.0);
}

std::vector<std::vector<float>> KoskoNetwork::activateMatrix(const std::vector<std::vector<float>>& mat) {
    int rows = mat.size();
    int cols = mat[0].size();

    std::vector<std::vector<float>> activated(rows, std::vector<float>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            activated[i][j] = activation(mat[i][j]);
        }
    }

    return activated;
}

std::vector<std::vector<float> > KoskoNetwork::divideMatrixByNumber(const std::vector<std::vector<float> > &mat, float number)
{
    int rows = mat.size();
    int cols = mat[0].size();

    std::vector<std::vector<float>> divided(rows, std::vector<float>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            divided[i][j] = mat[i][j] / number;
        }
    }

    return divided;
}

std::vector<std::vector<float>> KoskoNetwork::transposeMatrix(const std::vector<std::vector<float>>& mat) {
    int rows = mat.size();
    int cols = mat[0].size();

    std::vector<std::vector<float>> transposed(cols, std::vector<float>(rows));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposed[j][i] = mat[i][j];
        }
    }

    return transposed;
}

std::vector<std::vector<float>> KoskoNetwork::addMatrix(const std::vector<std::vector<float>>& mat1,
                                                           const std::vector<std::vector<float>>& mat2)
{
    int rows = mat1.size();
    int cols = mat1[0].size();

    if (rows != mat2.size() || cols != mat2[0].size()) {
        throw std::invalid_argument("Несовместимые размерности матриц");
    }

    std::vector<std::vector<float>> result(rows, std::vector<float>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }

    return result;
}

std::vector<std::vector<float>> KoskoNetwork::multiplyMatrix(const std::vector<std::vector<float>>& mat1,
                                                                const std::vector<std::vector<float>>& mat2)
{
    int rows1 = mat1.size();
    int cols1 = mat1[0].size();
    int rows2 = mat2.size();
    int cols2 = mat2[0].size();

    if (cols1 != rows2) {
        throw std::invalid_argument("Несовместимые размерности матриц");
    }

    std::vector<std::vector<float>> result(rows1, std::vector<float>(cols2));

    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            for (int k = 0; k < cols1; ++k) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }

    return result;
}

bool KoskoNetwork::matrixAreEqual(const std::vector<std::vector<float> > &mat1, const std::vector<std::vector<float> > &mat2)
{
    int rows = mat1.size();
    int cols = mat1[0].size();

    if (rows != mat2.size() || cols != mat2[0].size()) {
        return false;
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (mat1[i][j] != mat2[i][j]) {
                return false;
            }
        }
    }

    return true;
}

std::vector<std::vector<float> > KoskoNetwork::getClasses() const
{
    return classes;
}

void KoskoNetwork::train(int neuronCount, std::vector<std::vector<float>> input)
{
    int powerOfTwo = -1;
    for (int i = input.size(); i != 0; i >>= 1) {
        powerOfTwo++;
    }
    weights = std::vector<std::vector<float>>(powerOfTwo + 1, std::vector<float>(neuronCount));
    classes = std::vector<std::vector<float>>(input.size(), std::vector<float>(powerOfTwo + 1, -1));
    for (int i = 0; i < input.size(); i++) {
        int offset = 0;
        for (int j = i; j != 0; j >>= 1) {
            classes[i][powerOfTwo - offset] = j & 1;
            if (!classes[i][powerOfTwo - offset]) {
                classes[i][powerOfTwo - offset] = -1;
            }
            offset++;
        }
    }

    std::vector<std::vector<std::vector<float>>> matrixInput(input.size());
    std::vector<std::vector<std::vector<float>>> matrixClasses(classes.size());
    for (int i = 0; i < input.size(); i++) {
        matrixInput[i].push_back(input[i]);
        matrixClasses[i].push_back(classes[i]);
        weights = addMatrix(weights, multiplyMatrix(transposeMatrix(matrixClasses[i]), matrixInput[i]));
    }
    for (int i = 0; i < weights.size(); i++) {
        weights[i][i] = 0;
    }
    weights = divideMatrixByNumber(weights, input.size());
}

std::vector<float> KoskoNetwork::classify(std::vector<float> input)
{
    std::vector<std::vector<float>> matrixInput;
    matrixInput.push_back(input);

    std::vector<std::vector<float>> result(activateMatrix(multiplyMatrix(weights, transposeMatrix(matrixInput))));
    std::vector<std::vector<float>> lastResult;

    int iterations = 0;
    do {
        lastResult = result;
        result = activateMatrix(multiplyMatrix(weights,
                                activateMatrix(multiplyMatrix(transposeMatrix(weights), lastResult))));
        iterations++;
    } while (!matrixAreEqual(result, lastResult) && iterations < 1000);

    result = transposeMatrix(result);

    return result[0];
}
