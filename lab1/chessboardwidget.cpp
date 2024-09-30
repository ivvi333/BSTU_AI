#include "chessboardwidget.h"

ChessboardWidget::ChessboardWidget(Solution *s, QWidget *parent)
    : QWidget{parent}, solution{s}, isDrawingSolution{false}
{}

ChessboardWidget::~ChessboardWidget()
{
    delete solution;
}

Solution *ChessboardWidget::getSolution()
{
    return solution;
}

void ChessboardWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(QColor(50, 50, 50));
    painter.drawRect(rect());
    painter.setBrush(QColor(255, 255, 255));

    auto queenRows = solution->getQueenRows();
    int queenCount{queenRows.size()};

    QSizeF cellSize{(double)width() / queenCount, (double)height() / queenCount};

    for (int i = 0; i < queenCount; i++) {
        for (int j = i % 2; j < queenCount; j+=2) {
            painter.drawRect(i * cellSize.width(), j * cellSize.height(),
                             cellSize.width(), cellSize.height());
        }
    }

    if (isDrawingSolution) {
        painter.setBrush(QColor(255, 0, 0));
        for (int j = 0; j < queenCount; j++) {
            painter.drawEllipse(queenRows[j] * cellSize.width(), j * cellSize.height(),
                                 cellSize.width(), cellSize.height());
        }
    }
}
