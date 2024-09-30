#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <vector>

#include "graph.h"
#include "aco.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_numVerticesSpinBox_valueChanged(int arg1);

    void on_alphaSpinBox_valueChanged(int arg1);

    void on_betaSpinBox_valueChanged(int arg1);

    void on_rhoSpinBox_valueChanged(double arg1);

    void on_tau0SpinBox_valueChanged(double arg1);

    void on_numIterationSpinBox_valueChanged(int arg1);

    void on_solveButton_clicked();

    void on_regenerateButton_clicked();

private:
    void constructGraphScene();
    void drawLines();
    void drawNodes();
    void drawBestPath();

    Ui::MainWindow *ui;
    static const int nodeW = 50, nodeH = 50, scale = 8;
    QGraphicsTextItem *bestLengthText;
    QGraphicsScene *graphScene;

    QSharedPointer<Graph> graph;
    QSharedPointer<ACO> aco;
};
#endif // MAINWINDOW_H
