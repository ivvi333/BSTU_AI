#include "nnrectitem.h"

NNRectItem::NNRectItem(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{}

NNRectItem::NNRectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, width, height, parent)
{}

void NNRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}
