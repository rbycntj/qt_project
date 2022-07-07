#include "bstwindow.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QMenuBar>
#include <QSpacerItem>
#include <qglobal.h>
#include <QTime>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QStringListIterator>

BSTWindow::BSTWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QString directory = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/BSTVisualizer";
    if (!QDir(directory).exists())
        QDir().mkdir(directory);

    this->bst = this->getBST();

    this->createMenu();

    //在窗口底部构建组件
    deleteButton = new QPushButton("删除", this);
    insertButton = new QPushButton("插入", this);
    zoomInButton = new QPushButton("放大", this);
    zoomOutButton = new QPushButton("缩小", this);
    returnButton = new QPushButton("返回",this);

    insertValueLineEdit = new QLineEdit;
    deleteValueLineEdit = new QLineEdit;
    statusLabel = new QLabel;

    //设置按钮组件
    deleteButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    insertButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    zoomInButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    zoomOutButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    returnButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    insertValueLineEdit->setFixedWidth(200);
    insertValueLineEdit->setToolTip("输入单个值或多个被空格分开的值");

    deleteValueLineEdit->setFixedWidth(100);
    deleteValueLineEdit->setToolTip("输入你想删除的值");

    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteClicked()));
    connect(insertButton, SIGNAL(clicked()), this, SLOT(insertClicked()));
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomInClicked()));
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOutClicked()));
    connect(insertValueLineEdit, SIGNAL(returnPressed()), this, SLOT(insertClicked()));
    connect(deleteValueLineEdit, SIGNAL(returnPressed()), this, SLOT(deleteClicked()));
    connect(returnButton,&QPushButton::clicked,this,[=]{
        this->close();
        MainWindow* m = new MainWindow;
        m->show();
    });

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(deleteValueLineEdit);
    buttonLayout->addWidget(insertButton);
    buttonLayout->addWidget(insertValueLineEdit);
    buttonLayout->addSpacing(25);
    buttonLayout->addWidget(statusLabel);
    buttonLayout->addStretch(0);
    buttonLayout->addWidget(zoomInButton);
    buttonLayout->addWidget(zoomOutButton);
    buttonLayout->addWidget(returnButton);

    renderArea = new RenderArea(this->bst);

    treeScrollArea = new QScrollArea;
    treeScrollArea->setWidget(renderArea);
    treeScrollArea->installEventFilter(renderArea);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(treeScrollArea);
    mainLayout->addLayout(buttonLayout);

    centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    this->setCentralWidget(centralWidget);
    this->setMinimumHeight(400);
    this->setWindowTitle("二叉搜索树");

    this->show();
}

void BSTWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   this->renderArea->callRepaint();
}

BSTWindow::~BSTWindow()
{
    delete renderArea;
    delete deleteButton;
    delete insertButton;
    delete zoomInButton;
    delete zoomOutButton;
    delete treeScrollArea;
    delete bst;
    delete centralWidget;
}

void BSTWindow::createMenu()
{
    this->createActions();

    editMenu = this->menuBar()->addMenu(tr("&编辑"));
    editMenu->addAction(resetAction);
    editMenu->addAction(changeNodeColorAction);
    editMenu->addAction(changeBackgroundColorAction);
    editMenu->addAction(changeTextColorAction);

}

void BSTWindow::createActions()
{
    resetAction = new QAction(tr("&重置"), this);
    resetAction->setStatusTip("重置树为空");
    connect(resetAction, &QAction::triggered, this, &BSTWindow::resetMenu);

    changeNodeColorAction = new QAction(tr("结点颜色"), this);
    changeNodeColorAction->setStatusTip("改变结点颜色");//Change Node Color
    connect(changeNodeColorAction, &QAction::triggered, this, &BSTWindow::changeNodeColorMenu);

    changeBackgroundColorAction = new QAction(tr("背景颜色"), this);
    changeBackgroundColorAction->setStatusTip("改变背景颜色");
    connect(changeBackgroundColorAction, &QAction::triggered, this, &BSTWindow::changeBackgroundColorMenu);

    changeTextColorAction = new QAction(tr("结点内文本颜色"), this);
    changeTextColorAction->setStatusTip("改变结点内文本颜色");
    connect(changeTextColorAction, &QAction::triggered, this, &BSTWindow::changeTextColorMenu);

}

void BSTWindow::deleteClicked() const {
    QString value = deleteValueLineEdit->text();

    if(!this->bst->deleteItem(value.toInt()))
        this->statusLabel->setText("该值不在树中");
    else
        this->statusLabel->setText("该值被删除");

    this->renderArea->repaint();
    this->deleteValueLineEdit->setText("");
    return;
}

void BSTWindow::insertClicked() const
{
    QString values = insertValueLineEdit->text();
    QStringList valueList = values.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    QStringListIterator iterator(valueList);

    while (iterator.hasNext())
    {
        if(!this->bst->insert(iterator.next().toInt()))
            this->statusLabel->setText("重复的值");
        else
            this->statusLabel->setText("值成功插入");
    }
    this->renderArea->repaint();
    insertValueLineEdit->setText("");
    return;
}

void BSTWindow::zoomInClicked() const {
    this->statusLabel->setText("放大");
    renderArea->zoomIn();
    return;
}

void BSTWindow::zoomOutClicked() const {
    this->statusLabel->setText("缩小");
    renderArea->zoomOut();
    return;
}

void BSTWindow::resetMenu() const
{
    this->statusLabel->setText("重置树");
    this->bst->resetTree();
    this->renderArea->repaint();
    return;
}

void BSTWindow::changeNodeColorMenu()
{
    QColor color = QColorDialog::getColor(Qt::black, this);
    if (color.isValid())
    {
        this->renderArea->changeNodeColor(color);
        this->renderArea->repaint();
    }
    return;
}

void BSTWindow::changeBackgroundColorMenu()
{
    QColor color = QColorDialog::getColor(Qt::black, this);
    if (color.isValid())
    {
        QPalette pal(this->treeScrollArea->palette());
        pal.setColor(QPalette::Window, color);
        this->treeScrollArea->setPalette(pal);
        this->renderArea->changeBackgroundColor(color);
    }
    return;
}

void BSTWindow::changeTextColorMenu()
{
    QColor color = QColorDialog::getColor(Qt::black, this);
    if (color.isValid())
    {
        this->renderArea->changeTextColor(color);
        this->renderArea->repaint();
    }
    return;
}

BinarySearchTree<int>* BSTWindow::getBST()
{

    BinarySearchTree<int> *bst = new BinarySearchTree<int>;

    QString fileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/BSTVisualizer/last_bst.txt";

    QString text;
    QFile file(fileName);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return bst;
    }

    QTextStream reader(&file);

    while (!reader.atEnd())
    {
        reader >> text;
        if (text != " " && text != "")
            bst->insert(text.toInt());
    }

    file.close();
    return bst;
}
