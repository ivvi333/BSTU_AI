#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    generateInput();
    nn = QSharedPointer<KoskoNetwork>(new KoskoNetwork());

    const char *birds[8] = {"—", "|", "/", "\\", "^", "v", "<", ">"};
    recognitionTableModel = QSharedPointer<QStandardItemModel>(new QStandardItemModel(6, 2));
    recognitionTableModel->setHorizontalHeaderLabels({"Птица", "Вероятность"});
    for (int i = 0; i < 6; i++) {
        recognitionTableModel->setItem(i, 0, new QStandardItem(QString(birds[i])));
    }
    ui->recognizeTableView->setModel(recognitionTableModel.get());
    ui->recognizeTableView->verticalHeader()->setVisible(false);
    ui->recognizeTableView->resizeColumnsToContents();
    ui->recognizeTableView->resizeRowsToContents();

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    imageScene = nullptr;
    constructImageScene();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_learnPushButton_clicked()
{
    nn->train(input[0].size(), input);
    ui->recognizePushButton->setEnabled(true);
}

void MainWindow::constructImageScene()
{
    if (imageScene != nullptr) {
        delete imageScene;
    }
    int lineSizeInRect = 3;
    imageScene = new NNImageScene(lineSizeInRect, ui->graphicsView);
    ui->graphicsView->setScene(imageScene);

    for (int i = 0; i < lineSizeInRect + 1; i++) {
        imageScene->addLine(0, i * 50, 50 * lineSizeInRect, i * 50);
        imageScene->addLine(i * 50, 0, i * 50, 50 * lineSizeInRect);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    ui->graphicsView->fitInView(imageScene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_clearPushButton_clicked()
{
    imageScene->removeAllNNRect();
}


void MainWindow::on_horizontalPushButton_clicked()
{
    imageScene->drawHorizontal();
}


void MainWindow::on_verticalPushButton_clicked()
{
    imageScene->drawVertical();
}


void MainWindow::on_upRightPushButton_clicked()
{
    imageScene->drawUpRight();
}


void MainWindow::on_upLeftPushButton_clicked()
{
    imageScene->drawUpLeft();
}


void MainWindow::on_upPushButton_clicked()
{
    imageScene->drawUp();
}


void MainWindow::on_downPushButton_clicked()
{
    imageScene->drawDown();
}


void MainWindow::on_leftPushButton_clicked()
{
    imageScene->drawLeft();
}


void MainWindow::on_rightPushButton_clicked()
{
    imageScene->drawRight();
}


void MainWindow::on_recognizePushButton_clicked()
{
    std::vector<float> sceneInput = imageScene->getInput();
    std::vector<float> vectorOutput = nn->classify(sceneInput);
    std::vector<std::vector<float>> classes = nn->getClasses();
    std::vector<int> output;
    for (int i = 0; i < input.size(); i++) {
        output.push_back(vectorOutput == classes[i]);
    }

    std::vector<int>::iterator maxIter = std::max_element(output.begin(), output.end());
    int maxPos = std::distance(output.begin(), maxIter);
    for (int i = 0; i < 6; i++) {
        QStandardItem *item = new QStandardItem(QString::number(output[i]));
        if (i == maxPos) {
            item->setBackground(QBrush(QColor(220, 235, 239)));
        }
        recognitionTableModel->setItem(i, 1, item);
    }
}

void MainWindow::generateInput()
{
    input = {
        {-1, -1, -1, 1, 1, 1, -1, -1, -1},
        {-1, 1, -1, -1, 1, -1, -1, 1, -1},
        {-1, -1, 1, -1, 1, -1, 1, -1, -1},
        {1, -1, -1, -1, 1, -1, -1, -1, 1},
        {-1, 1, -1, 1, -1, 1, 1, -1, 1},
        {1, -1, 1, 1, -1, 1, -1, 1, -1}
    };
}

