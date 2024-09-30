#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>

class NeuralNetwork
{
public:
    NeuralNetwork(std::vector<unsigned> topology, float learnRate, unsigned numEpoch);
    ~NeuralNetwork();

    void train(std::vector<float*> input, std::vector<float*> output);

    std::vector<float> classify(float* input);

    void setLearnRate(float newLearnRate);

    void setNumEpoch(unsigned int newNumEpoch);

private:
    void propagateForward(const float *input);
    void propagateBackward(const float *output);
    void calculateErrors(const float *output);
    void updateWeights();
    float sigmoid(float x);
    float sigmoidDerivative(float x);

    unsigned numEpoch;
    std::vector<unsigned> topology;
    float learnRate;
    std::vector<float**> weights;
    std::vector<float*> neuronLayers, deltas;
};

#endif // NEURALNETWORK_H
