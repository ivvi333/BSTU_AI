#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Solution *solution = new Solution(ui->queenCountSpinBox->value(), ui->maxTempSpinBox->value(),
                     ui->minTempSpinBox->value(), ui->tempCoeffSpinBox->value(),
                     ui->stepCountSpinBox->value());
    chessboard = QSharedPointer<ChessboardWidget>(new ChessboardWidget(solution, this));

    ui->solutionStackedWidget->addWidget(chessboard.get());
    currWidget = 0;

    badSolutionChartView = QSharedPointer<QtCharts::QChartView>(new QtCharts::QChartView);
    badSolutionLineChart = QSharedPointer<QtCharts::QLineSeries>(new QtCharts::QLineSeries);
    badSolutionChartView->chart()->setTitle("График изменения принятых плохих решений");

    bestEnergyChartView = QSharedPointer<QtCharts::QChartView>(new QtCharts::QChartView);
    bestEnergyLineChart = QSharedPointer<QtCharts::QLineSeries>(new QtCharts::QLineSeries);
    bestEnergyChartView->chart()->setTitle("График изменения энергии лучшего решения");

    currTempChartView = QSharedPointer<QtCharts::QChartView>(new QtCharts::QChartView);
    currTempLineChart = QSharedPointer<QtCharts::QLineSeries>(new QtCharts::QLineSeries);
    currTempChartView->chart()->setTitle("График изменения температуры");

    ui->chartLayout->addWidget(badSolutionChartView.get());
    ui->chartLayout->addWidget(bestEnergyChartView.get());
    ui->chartLayout->addWidget(currTempChartView.get());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_maxTempSpinBox_valueChanged(double arg1)
{
    chessboard->getSolution()->setMaxTemp(arg1);
}


void MainWindow::on_minTempSpinBox_valueChanged(double arg1)
{
    chessboard->getSolution()->setMinTemp(arg1);
}


void MainWindow::on_tempCoeffSpinBox_valueChanged(double arg1)
{
    chessboard->getSolution()->setTempReductionRate(arg1);
}


void MainWindow::on_stepCountSpinBox_valueChanged(int arg1)
{
    chessboard->getSolution()->setStepCount(arg1);
}


void MainWindow::on_queenCountSpinBox_valueChanged(int arg1)
{
    chessboard->getSolution()->setQueenCount(arg1);
    chessboard->isDrawingSolution = false;
    chessboard->repaint();
}


void MainWindow::on_solveButton_clicked()
{
    badSolutionChartView->chart()->removeSeries(badSolutionLineChart.get());
    badSolutionLineChart->clear();
    bestEnergyChartView->chart()->removeSeries(bestEnergyLineChart.get());
    bestEnergyLineChart->clear();
    currTempChartView->chart()->removeSeries(currTempLineChart.get());
    currTempLineChart->clear();

    chessboard->getSolution()->annealingSolve(badSolutionLineChart.get(),
                                              bestEnergyLineChart.get(),
                                              currTempLineChart.get());

    badSolutionChartView->chart()->addSeries(badSolutionLineChart.get());
    badSolutionChartView->chart()->createDefaultAxes();
    bestEnergyChartView->chart()->addSeries(bestEnergyLineChart.get());
    bestEnergyChartView->chart()->createDefaultAxes();
    currTempChartView->chart()->addSeries(currTempLineChart.get());
    currTempChartView->chart()->createDefaultAxes();
    chessboard->isDrawingSolution = true;
    chessboard->repaint();
}


void MainWindow::on_switchButton_clicked()
{
    currWidget = (currWidget + 1) % 2;
    ui->solutionStackedWidget->setCurrentIndex(currWidget);
    if (currWidget) {
        ui->switchButton->setText("Показать данные");
    }
    else {
        ui->switchButton->setText("Показать доску");
    }
}

