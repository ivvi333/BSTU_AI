#include "graph.h"

#include <random>
#include <algorithm>
#include <cmath>

Graph::Graph(int numVertices)
{
    for (int i = 0; i < numVertices; i++)
        addVertex();
}

void Graph::regenerateVertices()
{
    int numVertices = V.size();
    for (int i = 0; i < numVertices; i++) {
        removeLastVertex();
    }
    for (int i = 0; i < numVertices; i++) {
        addVertex();
    }
}

void Graph::addVertex()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> coordDist(minCoordValue, maxCoordValue);

    Point p{coordDist(gen), coordDist(gen)};
    while (std::find(V.begin(), V.end(), p) != V.end())
    {
        p.x = coordDist(gen);
        p.y = coordDist(gen);
    }

    int numVertices = V.size();
    V.push_back(p);
    A.push_back(std::vector<int>(numVertices + 1));
    for (int i = 0; i < numVertices; i++) {
        double root = sqrt(pow(V[i].x - V[numVertices].x, 2.0) + pow(V[i].y - V[numVertices].y, 2.0));
        A[i].push_back((int)ceil(root));
        A[numVertices][i] = A[i][numVertices];
    }
}

void Graph::removeLastVertex()
{
    V.pop_back();
    A.pop_back();
    for (int i = 0; i < V.size(); i++) {
        A[i].pop_back();
    }
}
