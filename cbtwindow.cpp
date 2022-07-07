#include "cbtwindow.h"
#include "mainwindow.h"
#include "ui_cbtwindow.h"
#include <QIntValidator>


struct stackNode
{
    Node* treeNode = nullptr;
    int layer;      // 标记该节点属于第几层
};


CBTWindow::CBTWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TreeWindow)
{
    ui->setupUi(this);
    createCBT();
}

void CBTWindow::createCBT()
{
    tree = new CBTTree;
    QIntValidator* aIntValidator = new QIntValidator;
    ui->addEdit->setValidator(aIntValidator);
    ui->delEdit->setValidator(aIntValidator);
    connect(ui->addBtn,&QPushButton::clicked,this,[=]{
        QString strNum = ui->addEdit->text();
        int num = strNum.toInt();
        tree->insertNode(new Node(num));

        //setMyPoints();
        this->repaint();
    });

    connect(ui->delBtn,&QPushButton::clicked,this,[=]{
        QString strNum = ui->delEdit->text();
        int num = strNum.toInt();
        tree->deleteNode(new Node(num));
        QVector<int> temp(tree->v);
        tree = new CBTTree;
        for(int i = 0;i<temp.size();i++){
            tree->insertNode(new Node(temp[i]));
        }
        this->repaint();
    });

    connect(ui->retBtn,&QPushButton::clicked,this,[=]{
        this->close();
        MainWindow* m = new MainWindow;
        m->show();
    });
}

void CBTWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 设置画笔颜色、宽度
    painter.setPen(QPen(QColor(0, 160, 230), 2));
    // 设置画刷颜色
    //painter.setBrush(QColor(255, 160, 90));
    // 绘制圆
    qreal W = this->width();// 画布的宽
    qreal H = this->height();// 画布的高
    int treeHeight = tree->height;// 树的高度
    qDebug()<<"height:"<<treeHeight;
    qreal R = W / (2 * std::pow(2, treeHeight) + 2);// 节点的半径

    const int layerHeight = (H-4*R) / (treeHeight-1);// 层高，即垂直偏移量

    // 初始化
    // 节点的定义
    QRectF node(QPointF(-R, -R), QPointF(R, R));
    QStack<stackNode> stack;// 存放右孩子节点
    stackNode qNode;

    QStack<QPointF> points;// 存放右孩子节点相对于当前坐标系统原点的位置
    QPointF point;

    qreal Hoffset;// 水平偏移量
    Node* t = tree->root;
    if(t==nullptr){
        return;
    }
    const qreal Pi = 3.14159;
    int curLayer = 1;
    int curAngle;// 当前角度
    qreal deg;// 当前弧度

    painter.translate(W/2, 2*R);

    while (1)
    {
        painter.drawEllipse(node);
        painter.drawText(node, Qt::AlignCenter, QString::number(t->num));
        // 设置孩子节点相对于父节点的水平偏移量
        Hoffset = std::pow(2, (treeHeight - curLayer)) * R;
        deg = std::atan(Hoffset / layerHeight);// 返回的是弧度
        curAngle = 180 / Pi * deg;// 将弧度转化为角度

        if (t->right != NULL)
        {
            // 坐标轴逆时针旋转
            painter.rotate(-curAngle);

            //绘制图形路径
            painter.drawLine(0, R, 0, layerHeight / std::cos(deg) - R);

            // 旋转复原
            painter.rotate(curAngle);

            // 右孩子节点压栈
            qNode.treeNode = t->right;
            qNode.layer = curLayer + 1;
            stack.push(qNode);

            // 右孩子相对于当前坐标系统原点的位置压栈
            points.push(QPointF(QPointF(0, 0) + QPointF(Hoffset, layerHeight)));
            painter.save();
        }
        if (t->left != NULL)
        {
            // 坐标轴顺时针旋转
            painter.rotate(curAngle);
            // 绘制边
            painter.drawLine(0, R, 0, layerHeight / std::cos(deg) - R);
            // 旋转复原
            painter.rotate(-curAngle);
            // 原点移动到左孩子节点的位置
            painter.translate(QPointF(QPointF(0, 0) + QPointF(-Hoffset, layerHeight)));
            t = t->left;
            // 层次加1
            curLayer++;
        }
        else {
            if (!stack.empty()) {
                // 获取到右节点的层次状态
                qNode = stack.pop();
                t = qNode.treeNode;
                curLayer = qNode.layer;

                // 原点移动到右孩子节点的位置
                painter.restore();
                point = points.pop();
                painter.translate(point);
            }
            else {
                painter.resetTransform();
                return;
            }

        }
    }

}



CBTWindow::~CBTWindow()
{
    delete ui;
}
