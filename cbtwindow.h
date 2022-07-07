#ifndef CBTWINDOW_H
#define CBTWINDOW_H
#define PI 3.14

#include <QMainWindow>
#include <QQueue>
#include <QStack>
#include <QPainter>
#include <QPoint>
#include <QLine>
#include <QIcon>
#include <QDebug>
#include <math.h>

namespace Ui {
class TreeWindow;
}

struct Node
{
public:
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    int num = 0;
    QPoint point;
    Node(int num){
        this->num = num;
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
    }
};

struct CBTTree
{
public:
    Node* root = nullptr;
    QQueue<Node*>* queue = new QQueue<Node*>;
    int nodeSize = 0;
    int height = 0;
    QVector<int> v;

    void insertNode(Node* node){
        this->nodeSize++;
        if(root==nullptr){
            root = node;
            queue->push_back(node);
        }else{
            int size = queue->size();
            qDebug()<<"size:"<<size;
            for(int i = 0;i<size;i++){
                Node* curNode = queue->at(i);
                if(curNode->left==nullptr){
                    curNode->left = node;
                    node->parent = curNode;
                    queue->push_back(node);
                    break;
                }

                if(curNode->right==nullptr){
                    curNode->right = node;
                    node->parent = curNode;
                    queue->push_back(node);
                    break;
                }
            }

        }

        int temp = nodeSize;
        int h = 0;
        while(nodeSize!=0){
            nodeSize/=2;
            h++;
        }
        nodeSize = temp;

        this->height = h;
    }

    void deleteNode(Node* node){
        if(root==nullptr){
            return;
        }else{
            int size = queue->size();
            for(int i = 0;i<size;i++){
                Node* curNode = queue->at(i);
                if(curNode->num==node->num){
                    this->nodeSize--;
                    continue;
                }
                v.push_back(curNode->num);
            }
        }
    }
};


class CBTWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CBTWindow(QWidget *parent = nullptr);
    ~CBTWindow();
    void createCBT();
    void draw(Node *node, int x, int y, int angle, bool isLeft, int depth, QPainter *p);
    void setMyPoints();
    void paintEvent(QPaintEvent *event);

    CBTTree* tree = nullptr;
    QVector<QPoint> myPoints;
    QVector<QLine> myLines;
private:
    Ui::TreeWindow *ui;
};

#endif // CBTWINDOW_H
