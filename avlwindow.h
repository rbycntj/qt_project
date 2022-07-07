#ifndef AVLWINDOW_H
#define AVLWINDOW_H

#include <QPoint>
#include <QLine>
#include <QMainWindow>
#include "avltree.h"

namespace Ui {
    class AVLWindow;
}

struct pnt
{
    int val;
    QPoint point;
    pnt(int _val, QPoint _point) : val(_val), point(_point) {}
};

class AVLWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AVLWindow(QWidget* parent = 0);
    ~AVLWindow();

private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_clearButton_clicked();

private:
    void RebuildTree();

    void recursiveBuilder(node* n, int heightLevel, int centerDistance);

    AVLTree* tree;
    std::vector<pnt> points;
    std::vector<QLine> lines;
    std::list<int> valuesCache;
    Ui::AVLWindow* ui;
};

#endif
