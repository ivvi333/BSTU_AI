#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "tabledelegate.h"
#include <QColor>
#include <QBrush>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int numFeatureVectors = ui->maxFeatureVectorsSpinBox->value(),
        numFeatures = ui->maxFeaturesSpinBox->value(),
        numPrototypeVectors = ui->maxPrototypeVectorsSpinBox->value(),
        beta = ui->betaSpinBox->value();
    double vigilance = ui->vigilanceSpinBox->value();

    ui->tableWidget->setItemDelegate(new TableDelegate);
    ui->tableWidget->setRowCount(numFeatureVectors);
    ui->tableWidget->setColumnCount(numFeatures);

    art = QSharedPointer<ART>(new ART(numFeatureVectors, numFeatures, numPrototypeVectors, beta, vigilance));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    art->setFeature(item->row(), item->column(), item->text().toInt());
}

void MainWindow::on_maxFeatureVectorsSpinBox_valueChanged(int arg1)
{
    art->setMaxFeatureVectors(arg1);
    ui->tableWidget->setRowCount(arg1);
}


void MainWindow::on_maxFeaturesSpinBox_valueChanged(int arg1)
{
    art->setMaxFeatures(arg1);
    ui->tableWidget->setColumnCount(arg1);
}


void MainWindow::on_maxPrototypeVectorsSpinBox_valueChanged(int arg1)
{
    art->setMaxPrototypeVectors(arg1);
}


void MainWindow::on_betaSpinBox_valueChanged(int arg1)
{
    art->setBeta(arg1);
}

void MainWindow::on_vigilanceSpinBox_valueChanged(double arg1)
{
    art->setVigilance(arg1);
}

void MainWindow::on_solveButton_clicked()
{
    for (int i = 0; i < ui->maxFeatureVectorsSpinBox->value(); i++) {
        for (int j = 0; j < ui->maxFeaturesSpinBox->value(); j++) {
            if (ui->tableWidget->item(i, j)) {
                ui->tableWidget->item(i, j)->setBackground(QBrush(QColor(255, 255, 255)));
            }
        }
    }

    art->solve();
    std::vector<int> membership = art->getMembership();
    for (int i = 0; i < ui->maxFeatureVectorsSpinBox->value(); i++) {
        for (int j = 0; j < ui->maxFeaturesSpinBox->value(); j++) {
            if (ui->tableWidget->item(i, j) && membership[i] >= 0) {
                ui->tableWidget->item(i, j)->setBackground(QBrush(QColor(
                    ((membership[i] + 509) * 10001) % 255,
                    ((membership[i] + 209) * 10009) % 255,
                    ((membership[i] + 301) * 10103) % 255)
                ));
            }
        }
    }
}

