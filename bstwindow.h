#ifndef BSTWINDOW_H
#define BSTWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QColorDialog>
#include <QLineEdit>
#include <QColor>
#include "renderarea.h"
#include "binarysearchtree.h"

class BSTWindow : public QMainWindow
{
    Q_OBJECT

public:
    BSTWindow(QWidget *parent = nullptr);
    ~BSTWindow();

    QWidget *centralWidget = nullptr;
private:
    RenderArea *renderArea;
    QPushButton *deleteButton;
    QPushButton *insertButton;
    QPushButton *zoomInButton;
    QPushButton *zoomOutButton;
    QPushButton *returnButton;
    QLineEdit *insertValueLineEdit;
    QLineEdit *deleteValueLineEdit;
    QScrollArea *treeScrollArea;
    QVBoxLayout *mainLayout;
    QLabel *statusLabel;
    QMenu *editMenu;
    QAction *resetAction;
    QAction *changeNodeColorAction;
    QAction *changeBackgroundColorAction;
    QAction *changeTextColorAction;
    BinarySearchTree<int> *bst;
    BinarySearchTree<int> *getBST();
    void createMenu();
    void createActions();

protected:
    virtual void resizeEvent(QResizeEvent* event);

private slots:
    void insertClicked() const;
    void deleteClicked() const;
    void zoomInClicked() const;
    void zoomOutClicked() const;
    void resetMenu() const;
    void changeNodeColorMenu();
    void changeBackgroundColorMenu();
    void changeTextColorMenu();
};

#endif
