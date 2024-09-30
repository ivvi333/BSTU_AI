#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QChartView>
#include <QLineSeries>

#include "chessboardwidget.h"

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
    void on_maxTempSpinBox_valueChanged(double arg1);

    void on_minTempSpinBox_valueChanged(double arg1);

    void on_tempCoeffSpinBox_valueChanged(double arg1);

    void on_stepCountSpinBox_valueChanged(int arg1);

    void on_queenCountSpinBox_valueChanged(int arg1);

    void on_solveButton_clicked();

    void on_switchButton_clicked();

private:
    Ui::MainWindow *ui;
    QSharedPointer<ChessboardWidget> chessboard;
    QSharedPointer<QtCharts::QChartView> badSolutionChartView, bestEnergyChartView, currTempChartView;
    QSharedPointer<QtCharts::QLineSeries> badSolutionLineChart, bestEnergyLineChart, currTempLineChart;
    char currWidget;

    void annealingAlg();
};
#endif // MAINWINDOW_H
