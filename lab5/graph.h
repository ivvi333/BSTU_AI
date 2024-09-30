#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <QPoint>

class Graph
{
public:
    Graph(int numVertices);

    int getNumVertices() const { return V.size(); }

    void regenerateVertices();

    void addVertex();

    void removeLastVertex();

    const std::vector<std::vector<int>> &getA() const { return A; }

    const QPoint getNodeAsQPointByIndex(int index) const { return QPoint(V[index].x, V[index].y); }

private:
    struct Point {
        inline bool operator==(const Point& p) { return (x == p.x && y == p.y); }
        int x, y;
    };

    static const int minCoordValue = 0, maxCoordValue = 100;
    std::vector<Point> V;                // координаты вершин
    std::vector<std::vector<int>> A;     // матрица смежности
};

#endif // GRAPH_H
