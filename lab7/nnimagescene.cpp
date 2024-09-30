#include "nnimagescene.h"

NNImageScene::NNImageScene(int lineSizeInRect, QObject *parent)
    : lineSizeInRect{lineSizeInRect}, QGraphicsScene{parent}
{
    grid = new float*[lineSizeInRect];
    for (int i = 0; i < lineSizeInRect; i++) {
        grid[i] = new float[lineSizeInRect];
        for (int j = 0; j < lineSizeInRect; j++) {
            grid[i][j] = -1.0;
        }
    }
}

NNImageScene::~NNImageScene()
{
    for (int i = 0; i < lineSizeInRect; i++)
        delete[] grid[i];
    delete[] grid;
}

std::vector<float> NNImageScene::getInput() const
{
    std::vector<float> input;

    for (int y = 0; y < lineSizeInRect; y++) {
        for (int x = 0; x < lineSizeInRect; x++) {
            input.push_back(grid[y][x]);
        }
    }

    return input;
}

void NNImageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    int rectX = (int)event->scenePos().x() / 50,
        rectY = (int)event->scenePos().y() / 50;

    if (rectX < 0 || rectY < 0 || rectX >= lineSizeInRect || rectY >= lineSizeInRect) {
        return;
    }

    if (grid[rectY][rectX] == -1.0) {
        addNNRect(rectX, rectY);
    }
    else {
        removeNNRect(rectX, rectY);
    }
}

void NNImageScene::removeAllNNRect()
{
    for (int x = 0; x < lineSizeInRect; x++) {
        for (int y = 0; y < lineSizeInRect; y++) {
            removeNNRect(x, y);
        }
    }
}

void NNImageScene::drawHorizontal()
{
    removeAllNNRect();

    for (int x = 0; x < lineSizeInRect; x++) {
        addNNRect(x, lineSizeInRect / 2);
    }
}

void NNImageScene::drawVertical()
{
    removeAllNNRect();

    for (int y = 0; y < lineSizeInRect; y++) {
        addNNRect(lineSizeInRect / 2, y);
    }
}

void NNImageScene::drawUpRight()
{
    removeAllNNRect();

    for (int x = 0; x < lineSizeInRect; x++) {
        addNNRect(x, lineSizeInRect - 1 - x);
    }
}

void NNImageScene::drawUpLeft()
{
    removeAllNNRect();

    for (int x = 0; x < lineSizeInRect; x++) {
        addNNRect(x, x);
    }
}

void NNImageScene::drawUp()
{
    removeAllNNRect();

    for (int y = 0; y < lineSizeInRect; y++) {
        addNNRect(lineSizeInRect / 2 - (y / 2 + y % 2), y);
        addNNRect(lineSizeInRect / 2 + (y / 2 + y % 2), y);
    }
}

void NNImageScene::drawDown()
{
    removeAllNNRect();

    for (int y = 0; y < lineSizeInRect; y++) {
        addNNRect(lineSizeInRect / 2 - (y / 2 + y % 2), lineSizeInRect - 1 - y);
        addNNRect(lineSizeInRect / 2 + (y / 2 + y % 2), lineSizeInRect - 1 - y);
    }
}

void NNImageScene::drawLeft()
{
    removeAllNNRect();

    for (int x = 0; x < lineSizeInRect; x++) {
        addNNRect(x, lineSizeInRect / 2 - (x / 2 + x % 2));
        addNNRect(x, lineSizeInRect / 2 + (x / 2 + x % 2));
    }
}

void NNImageScene::drawRight()
{
    removeAllNNRect();

    for (int x = 0; x < lineSizeInRect; x++) {
        addNNRect(lineSizeInRect - 1 - x, lineSizeInRect / 2 - (x / 2 + x % 2));
        addNNRect(lineSizeInRect - 1 - x, lineSizeInRect / 2 + (x / 2 + x % 2));
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
    grid[y][x] = -1.0;
}
