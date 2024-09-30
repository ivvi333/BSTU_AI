#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int numVertices = ui->numVerticesSpinBox->value(),
        populationSize = ui->populationSizeSpinBox->value(),
        numGenerations = ui->numGenerationSpinBox->value();

    double mutationRate = ui->mutationRateSpinBox->value();

    graph = QSharedPointer<Graph>(new Graph(numVertices));
    ga = QSharedPointer<GA>(new GA(graph.get(), populationSize, mutationRate, numGenerations));

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    graphScene = nullptr;
    constructGraphScene();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_numVerticesSpinBox_valueChanged(int arg1)
{
    int numVertices = graph->getNumVertices();
    while (numVertices != arg1) {
        if (numVertices < arg1) {
            graph->addVertex();
            numVertices++;
        }
        else {
            graph->removeLastVertex();
            numVertices--;
        }
    }

    constructGraphScene();
}


void MainWindow::on_regenerateButton_clicked()
{
    graph->regenerateVertices();

    constructGraphScene();
}

void MainWindow::on_solveButton_clicked()
{
    ga->solve();

    drawBestPath();
}


void MainWindow::on_populationSizeSpinBox_valueChanged(int arg1)
{
    ga->setPopulationSize(arg1);
}


void MainWindow::on_mutationRateSpinBox_valueChanged(double arg1)
{
    ga->setMutationRate(arg1);
}

void MainWindow::on_numGenerationSpinBox_valueChanged(int arg1)
{
    ga->setNumGenerations(arg1);
}

void MainWindow::constructGraphScene()
{
    if (graphScene != nullptr) {
        delete graphScene;
    }
    graphScene = new QGraphicsScene(ui->graphicsView);

    drawLines();
    drawNodes();
    bestLengthText = nullptr;

    ui->graphicsView->setScene(graphScene);
}

void MainWindow::drawLines()
{
    if (graphScene == nullptr) {
        return;
    }

    for (int i = 0; i < graph->getNumVertices(); i++) {
        QPoint nc1 = graph->getNodeAsQPointByIndex(i);
        for (int j = 0; j < graph->getNumVertices(); j++) {
            if (i != j) {
                QPoint nc2 = graph->getNodeAsQPointByIndex(j);
                graphScene->addLine(nc1.x() * scale, nc1.y() * scale,
                                    nc2.x() * scale, nc2.y() * scale,
                                    QPen(QBrush(Qt::lightGray), 2));
            }
        }
    }
}

void MainWindow::drawNodes()
{
    if (graphScene == nullptr) {
        return;
    }

    for (int i = 0; i < graph->getNumVertices(); i++) {
        QPoint nc1 = graph->getNodeAsQPointByIndex(i);
        graphScene->addEllipse(nc1.x() * scale - nodeW / 2,
                               nc1.y() * scale - nodeH / 2,
                               nodeW, nodeH,
                               QPen(QBrush(Qt::black), 2), QBrush(Qt::blue));
    }
}

void MainWindow::drawBestPath()
{
    if (graphScene == nullptr) {
        return;
    }

    drawLines();

    std::vector<int> bestPath = ga->getBestPath();

    for (int i = 0; i < bestPath.size() - 1; i++) {
        QPoint nc1 = graph->getNodeAsQPointByIndex(bestPath[i]);
        QPoint nc2 = graph->getNodeAsQPointByIndex(bestPath[i + 1]);
        graphScene->addLine(nc1.x() * scale, nc1.y() * scale,
                            nc2.x() * scale, nc2.y() * scale,
                            QPen(QBrush(Qt::red), 2));
    }

    drawNodes();

    if (bestLengthText != nullptr) {
        graphScene->removeItem(bestLengthText);
        delete bestLengthText;
    }
    bestLengthText = graphScene->addText(QString::number(ga->getBestLength()), QFont("Courier", 36));
}

