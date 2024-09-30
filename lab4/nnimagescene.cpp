#include "nnimagescene.h"

NNImageScene::NNImageScene(QObject *parent)
    : QGraphicsScene{parent}
{
    grid = new float*[9];
    for (int i = 0; i < 9; i++) {
        grid[i] = new float[9];
        for (int j = 0; j < 9; j++) {
            grid[i][j] = 0.0;
        }
    }
}

NNImageScene::~NNImageScene()
{
    for (int i = 0; i < 9; i++)
        delete[] grid[i];
    delete[] grid;
}

std::vector<float> NNImageScene::getInput() const
{
    std::vector<float> input;

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            input.push_back(grid[y][x]);
        }
    }

    return input;
}

void NNImageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    int rectX = (int)event->scenePos().x() / 50,
        rectY = (int)event->scenePos().y() / 50;

    if (rectX < 0 || rectY < 0 || rectX >= 9 || rectY >= 9) {
        return;
    }

    if (grid[rectY][rectX] == 0.0) {
        addNNRect(rectX, rectY);
    }
    else {
        removeNNRect(rectX, rectY);
    }
}

void NNImageScene::removeAllNNRect()
{
    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            removeNNRect(x, y);
        }
    }
}

void NNImageScene::drawHorizontal()
{
    removeAllNNRect();

    for (int x = 0; x < 9; x++) {
        addNNRect(x, 4);
    }
}

void NNImageScene::drawVertical()
{
    removeAllNNRect();

    for (int y = 0; y < 9; y++) {
        addNNRect(4, y);
    }
}

void NNImageScene::drawUpRight()
{
    removeAllNNRect();

    for (int x = 0; x < 9; x++) {
        addNNRect(x, 8 - x);
    }
}

void NNImageScene::drawUpLeft()
{
    removeAllNNRect();

    for (int x = 0; x < 9; x++) {
        addNNRect(x, x);
    }
}

void NNImageScene::drawUp()
{
    removeAllNNRect();

    for (int y = 0; y < 9; y++) {
        addNNRect(4 - (y / 2 + y % 2), y);
        addNNRect(4 + (y / 2 + y % 2), y);
    }
}

void NNImageScene::drawDown()
{
    removeAllNNRect();

    for (int y = 0; y < 9; y++) {
        addNNRect(4 - (y / 2 + y % 2), 8 - y);
        addNNRect(4 + (y / 2 + y % 2), 8 - y);
    }
}

void NNImageScene::drawLeft()
{
    removeAllNNRect();

    for (int x = 0; x < 9; x++) {
        addNNRect(x, 4 - (x / 2 + x % 2));
        addNNRect(x, 4 + (x / 2 + x % 2));
    }
}

void NNImageScene::drawRight()
{
    removeAllNNRect();

    for (int x = 0; x < 9; x++) {
        addNNRect(8 - x, 4 - (x / 2 + x % 2));
        addNNRect(8 - x, 4 + (x / 2 + x % 2));
    }
}

void NNImageScene::addNNRect(int x, int y)
{
    this->addRect(50 * x, 50 * y, 50, 50, QPen(), QBrush(Qt::black));
    grid[y][x] = 1.0;
}

void NNImageScene::removeNNRect(int x, int y)
{
    this->addRect(50 * x, 50 * y, 50, 50, QPen(), QBrush(Qt::white));
    grid[y][x] = 0.0;
}
