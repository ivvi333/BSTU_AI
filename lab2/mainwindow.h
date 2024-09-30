#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QTableWidgetItem>

#include "art.h"

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
    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_maxFeatureVectorsSpinBox_valueChanged(int arg1);

    void on_maxFeaturesSpinBox_valueChanged(int arg1);

    void on_maxPrototypeVectorsSpinBox_valueChanged(int arg1);

    void on_betaSpinBox_valueChanged(int arg1);

    void on_vigilanceSpinBox_valueChanged(double arg1);

    void on_solveButton_clicked();

private:
    Ui::MainWindow *ui;
    QSharedPointer<ART> art;
};
#endif // MAINWINDOW_H
