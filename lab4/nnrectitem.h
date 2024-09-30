#ifndef NNRECTITEM_H
#define NNRECTITEM_H

#include <QGraphicsRectItem>

class NNRectItem : public QGraphicsRectItem
{
public:
    NNRectItem(QGraphicsItem *parent = nullptr);
    NNRectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // NNRECTITEM_H
