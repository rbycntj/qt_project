#include "avlwindow.h"
#include "ui_avlwindow.h"
#include <mainwindow.h>
#include <cmath>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QPainter>

int maxLevelWidth = 0;

AVLWindow::AVLWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::AVLWindow)
{
    ui->setupUi(this);
    tree = new AVLTree();
    ui->treeView->setScene(new QGraphicsScene);
    ui->treeView->setRenderHint(QPainter::Antialiasing, true);
    connect(ui->retBtn,&QPushButton::clicked,this,[=]{
        this->close();
        MainWindow* m = new MainWindow;
        m->show();
    });

}

AVLWindow::~AVLWindow()
{
    delete ui;
}

void AVLWindow::RebuildTree()
{
    ui->treeView->scene()->clear();
    points.clear();
    lines.clear();

    if (!tree->getRoot())
        return;
    // how to build a tree?
    // find maximum depth
    int maxDepth = tree->getRoot()->height;
    // compute width
    maxLevelWidth = maxDepth * 25      //ellipses
        + pow(2.0, maxDepth) * 10; //spaces
// build points recursive from root
    recursiveBuilder(tree->getRoot(), 1, 0);
    for (unsigned int i = 0; i < lines.size(); i++)
        ui->treeView->scene()->addLine(lines[i]);

    for (unsigned int i = 0; i < points.size(); i++)
    {
        QGraphicsTextItem* io = new QGraphicsTextItem;
        if (points[i].val < 10)
            io->setPos(points[i].point.x() + 6, points[i].point.y() + 2);
        else
            io->setPos(points[i].point.x() + 3, points[i].point.y() + 2);
        io->setPlainText(QString::number(points[i].val));
        ui->treeView->scene()->addItem(io);
        ui->treeView->scene()->addEllipse(points[i].point.x(), points[i].point.y(), 25, 25);
    }
    ui->treeView->setSceneRect(ui->treeView->scene()->sceneRect());
}

void AVLWindow::recursiveBuilder(node* n, int heightLevel, int xPos)
{
    QPoint curPos = QPoint(xPos, heightLevel * 50);
    points.push_back(pnt(n->key, curPos));

    int xShift = maxLevelWidth / pow(2.0, heightLevel + 1);

    if (n->left)
    {
        lines.push_back(QLine(QPoint(curPos.x() + 12, curPos.y() + 25),
            QPoint(xPos - xShift + 13, heightLevel * 50 + 50)));
        recursiveBuilder(n->left, heightLevel + 1, xPos - xShift);
    }
    if (n->right)
    {
        lines.push_back(QLine(QPoint(curPos.x() + 13, curPos.y() + 25),
            QPoint(xPos + xShift + 12, heightLevel * 50 + 50)));
        recursiveBuilder(n->right, heightLevel + 1, xPos + xShift);
    }
}

void AVLWindow::on_addButton_clicked()
{
    std::list<int>::iterator val = std::find(valuesCache.begin(),
        valuesCache.end(),
        ui->valueSpinBox->value());
    if (val == valuesCache.end())
    {
        tree->insertValue(ui->valueSpinBox->value());
        valuesCache.push_back(ui->valueSpinBox->value());
    }

    RebuildTree();
}

void AVLWindow::on_removeButton_clicked()
{
    std::list<int>::iterator val = std::find(valuesCache.begin(),
        valuesCache.end(),
        ui->valueSpinBox->value());
    if (val != valuesCache.end())
    {
        tree->removeValue(ui->valueSpinBox->value());
        valuesCache.remove(ui->valueSpinBox->value());
    }

    RebuildTree();
}

void AVLWindow::on_clearButton_clicked()
{
    tree->clearTree();
    valuesCache.clear();
    RebuildTree();
}
