#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    float learnRate = ui->learnRateSpinBox->value();
    unsigned numEpoch = ui->numEpochSpinBox->value();

    nn = QSharedPointer<NeuralNetwork>(new NeuralNetwork({81, 8, 8}, learnRate, numEpoch));

    const char *birds[8] = {"—", "|", "/", "\\", "^", "v", "<", ">"};
    recognitionTableModel = QSharedPointer<QStandardItemModel>(new QStandardItemModel(8, 2));
    recognitionTableModel->setHorizontalHeaderLabels({"Птица", "Вероятность"});
    for (int i = 0; i < 8; i++) {
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

void MainWindow::on_learnRateSpinBox_valueChanged(double arg1)
{
    nn->setLearnRate(arg1);
}


void MainWindow::on_numEpochSpinBox_valueChanged(int arg1)
{
    nn->setNumEpoch(arg1);
}


void MainWindow::on_learnPushButton_clicked()
{
    std::vector<float> input1 =
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        };
    std::vector<float> output1 = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    std::vector<float> input2 =
        {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0
        };
    std::vector<float> output2 = {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    std::vector<float> input3 =
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        };
    std::vector<float> output3 = {0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    std::vector<float> input4 =
        {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0
        };
    std::vector<float> output4 = {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};

    std::vector<float> input5 =
        {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0
        ,0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0
        ,0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0
        ,0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0
        ,1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0
        ,1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0
        };
    std::vector<float> output5 = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0};

    std::vector<float> input6 =
        {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0
        ,1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0
        ,0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0
        ,0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0
        ,0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0
        ,0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0
        };
    std::vector<float> output6 = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0};

    std::vector<float> input7=
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0
        };
    std::vector<float> output7 = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0};

    std::vector<float> input8 =
        {1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0
        ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0
        ,0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0
        ,0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0
        ,1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        };
    std::vector<float> output8 = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0};

    std::vector<float *> input{&input1[0], &input2[0], &input3[0], &input4[0],
                               &input5[0], &input6[0], &input7[0], &input8[0]};
    std::vector<float *> output{&output1[0], &output2[0], &output3[0], &output4[0],
                                &output5[0], &output6[0], &output7[0], &output8[0]};

    nn->train(input, output);
    ui->recognizePushButton->setEnabled(true);
}

void MainWindow::constructImageScene()
{
    if (imageScene != nullptr) {
        delete imageScene;
    }
    imageScene = new NNImageScene(ui->graphicsView);
    ui->graphicsView->setScene(imageScene);

    for (int i = 0; i < 10; i++) {
        imageScene->addLine(0, i * 50, 450, i * 50);
        imageScene->addLine(i * 50, 0, i * 50, 450);
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
    std::vector<float> input = imageScene->getInput();
    std::vector<float> output = nn->classify(&input[0]);
    std::vector<float>::iterator maxIter = std::max_element(output.begin(), output.end());
    int maxPos = std::distance(output.begin(), maxIter);
    for (int i = 0; i < 8; i++) {
        QStandardItem *item = new QStandardItem(QString::number(output[i]));
        if (i == maxPos) {
            item->setBackground(QBrush(QColor(220, 235, 239)));
        }
        recognitionTableModel->setItem(i, 1, item);
    }

}

