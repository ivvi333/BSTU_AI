#include "neuralnetwork.h"

#include <random>
#include <cmath>
// #include <algorithm>

NeuralNetwork::NeuralNetwork(std::vector<unsigned int> topology, float learnRate, unsigned numEpoch)
    : topology{topology}, learnRate{learnRate}, numEpoch{numEpoch}
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> weightDist(-1.0, 1.0);

    for (unsigned t = 0; t < topology.size(); t++) {
        if (t == topology.size() - 1) {
            neuronLayers.push_back(new float[topology[t]]);
            deltas.push_back(new float[topology[t]]);
        }
        else {
            // +1 нейрон смещения
            neuronLayers.push_back(new float[topology[t] + 1]);
            deltas.push_back(new float[topology[t] + 1]);
        }

        if (t != topology.size() - 1) {
            neuronLayers.back()[topology[t]] = 1.0;
        }

        if (t != 0) {
            if (t != topology.size() - 1) {
                weights.push_back(new float*[topology[t - 1] + 1]);
                for (unsigned i = 0; i <= topology[t - 1]; i++) {
                    weights.back()[i] = new float[topology[t] + 1];
                    for (unsigned j = 0; j < topology[t]; j++) {
                        weights.back()[i][j] = weightDist(gen);
                    }
                    // Другие нейроны НЕ ДОЛЖНЫ влиять на следующий нейрон смещения
                    weights.back()[i][topology[t]] = 0.0;
                }
                // Нейрон смещения ДОЛЖЕН влиять на следующий нейрон смещения
                weights.back()[topology[t - 1]][topology[t]] = 1.0;
            }

            else {
                weights.push_back(new float*[topology[t - 1] + 1]);
                for (unsigned i = 0; i <= topology[t - 1]; i++) {
                    weights.back()[i] = new float[topology[t]];
                    for (unsigned j = 0; j < topology[t]; j++) {
                        weights.back()[i][j] = weightDist(gen);
                    }
                }
            }
        }
    }
}

NeuralNetwork::~NeuralNetwork()
{
    for (unsigned t = 0; t < topology.size(); t++) {
        delete[] neuronLayers[t];
        delete[] deltas[t];
        if (t != 0) {
            for (unsigned i = 0; i <= topology[t - 1]; i++) {
                delete[] weights[t - 1][i];
            }
            delete[] weights[t - 1];
        }
    }
}

void NeuralNetwork::train(std::vector<float *> input, std::vector<float *> output)
{
    // Для каждого входного образа должен быть ожидаемый результат
    if (input.size() != output.size())
        return;

    for (unsigned e = 0; e < numEpoch; e++) {
        for (unsigned i = 0; i < input.size(); i++) {
            propagateForward(input[i]);
            // float *testOutput = neuronLayers.back();
            // float *maxAddr = std::max_element(testOutput, testOutput + topology.back());
            // unsigned maxPos = std::distance(testOutput, maxAddr);
            propagateBackward(output[i]);
        }
    }
}

std::vector<float> NeuralNetwork::classify(float* input)
{
    propagateForward(input);
    // float *testOutput = neuronLayers.back();
    // float *maxAddr = std::max_element(testOutput, testOutput + topology.back());
    // unsigned maxPos = std::distance(testOutput, maxAddr);
    float *output = neuronLayers.back();
    return std::vector<float>(output, output + topology.back());
}

void NeuralNetwork::propagateForward(const float *input)
{
    for (unsigned i = 0; i < topology[0]; i++) {
        neuronLayers[0][i] = input[i];
    }

    for (unsigned t = 1; t < topology.size(); t++) {
        for (unsigned j = 0; j < topology[t]; j++) {
            float sum = 0.0;
            for (unsigned i = 0; i <= topology[t - 1]; i++) {
                sum += neuronLayers[t - 1][i] * weights[t - 1][i][j];
            }
            neuronLayers[t][j] = sigmoid(sum);
        }
    }
}

void NeuralNetwork::propagateBackward(const float *output)
{
    calculateErrors(output);
    updateWeights();
}

void NeuralNetwork::calculateErrors(const float *output)
{
    for (unsigned i = 0; i < topology.back(); i++) {
        deltas.back()[i] = (output[i] - neuronLayers.back()[i]) * sigmoidDerivative(neuronLayers.back()[i]);
    }

    for (unsigned t = topology.size() - 2; t > 0; t--) {
        for (unsigned i = 0; i < topology[t]; i++) {
            deltas[t][i] = 0.0;

            for (unsigned j = 0; j < topology[t + 1]; j++) {
                deltas[t][i] += deltas[t + 1][j] * weights[t][i][j];
            }

            deltas[t][i] *= sigmoidDerivative(neuronLayers[t][i]);
        }
    }
}

void NeuralNetwork::updateWeights()
{
    for (unsigned t = topology.size() - 1; t > 0; t--) {
        for (unsigned j = 0; j < topology[t]; j++) {
            for (unsigned i = 0; i < topology[t - 1]; i++) {
                weights[t - 1][i][j] += (learnRate * deltas[t][j] * neuronLayers[t - 1][i]);
            }
            weights[t - 1][topology[t - 1]][j] += (learnRate * deltas[t][j]);
        }
    }
}

float NeuralNetwork::sigmoid(float x)
{
    return (1.0 / (1.0 + exp(-x)));
}

// Переданный сюда x УЖЕ ПРОШЁЛ ЧЕРЕЗ ФУНКЦИЮ АКТИВАЦИИ
// (для того, чтобы не хранить значения, полученные на нейронах до активации сигмоидой)
float NeuralNetwork::sigmoidDerivative(float x)
{
    return (x * (1.0 - x));
}

void NeuralNetwork::setNumEpoch(unsigned int newNumEpoch)
{
    numEpoch = newNumEpoch;
}

void NeuralNetwork::setLearnRate(float newLearnRate)
{
    learnRate = newLearnRate;
}
