#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QStandardItemModel>

#include "nnimagescene.h"
#include "neuralnetwork.h"

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
    void on_learnRateSpinBox_valueChanged(double arg1);

    void on_numEpochSpinBox_valueChanged(int arg1);

    void on_learnPushButton_clicked();

    void on_clearPushButton_clicked();

    void on_horizontalPushButton_clicked();

    void on_verticalPushButton_clicked();

    void on_upRightPushButton_clicked();

    void on_upLeftPushButton_clicked();

    void on_upPushButton_clicked();

    void on_downPushButton_clicked();

    void on_leftPushButton_clicked();

    void on_rightPushButton_clicked();

    void on_recognizePushButton_clicked();

private:
    void constructImageScene();
    void paintEvent(QPaintEvent *event);

    Ui::MainWindow *ui;
    NNImageScene *imageScene;
    QSharedPointer<QStandardItemModel> recognitionTableModel;
    QSharedPointer<NeuralNetwork> nn;
};
#endif // MAINWINDOW_H
