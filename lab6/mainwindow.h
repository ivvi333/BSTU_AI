#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QStandardItemModel>

#include "nnimagescene.h"
#include "hopfieldnetwork.h"

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
    void generateInput();
    void constructImageScene();
    void paintEvent(QPaintEvent *event);

    Ui::MainWindow *ui;
    NNImageScene *imageScene;
    QSharedPointer<QStandardItemModel> recognitionTableModel;
    QSharedPointer<HopfieldNetwork> nn;
    std::vector<std::vector<float>> input;
};
#endif // MAINWINDOW_H
