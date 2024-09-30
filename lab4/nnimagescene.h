#ifndef NNIMAGESCENE_H
#define NNIMAGESCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class NNImageScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit NNImageScene(QObject *parent = nullptr);
    ~NNImageScene();

    std::vector<float> getInput() const;
    void removeAllNNRect();
    void drawHorizontal();
    void drawVertical();
    void drawUpRight();
    void drawUpLeft();
    void drawUp();
    void drawDown();
    void drawLeft();
    void drawRight();

protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    float** grid;
    void addNNRect(int x, int y);
    void removeNNRect(int x, int y);
};

#endif // NNIMAGESCENE_H
