#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <sortwindow.h>
#include <cbtwindow.h>
#include <bstwindow.h>
#include <avlwindow.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->sortBtn,&QPushButton::clicked,this,[=]{
        this->close();
        SortWindow * s = new SortWindow;
        s->show();
});

    connect(ui->treeBtn,&QPushButton::clicked,this,[=]{
        switch (ui->sortingtype->currentIndex()) {
        case CBT:{
            this->close();
            CBTWindow * cbt = new CBTWindow;
            cbt->show();
            break;
        }
        case BST:{
            this->close();
            BSTWindow* bst = new BSTWindow;
            bst->show();
            break;
        }
        case AVL:{
            this->close();
            AVLWindow* avl = new AVLWindow;
            avl->show();
            break;
        }
        default:
            break;

        }
});
}

MainWindow::~MainWindow()
{
    delete ui;
}
