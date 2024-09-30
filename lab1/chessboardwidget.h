#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include <QWidget>
#include <QPainter>

#include "solution.h"

class ChessboardWidget : public QWidget
{
    Q_OBJECT
public:
    ChessboardWidget(Solution *s, QWidget *parent = nullptr);
    ~ChessboardWidget();

    bool isDrawingSolution;

    Solution *getSolution();

private:
    Solution *solution;

    void paintEvent(QPaintEvent *event);
};

#endif // CHESSBOARDWIDGET_H
