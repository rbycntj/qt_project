#include "sortwindow.h"
#include "ui_sortwindow.h"
#include "mainwindow.h"
#include <QtGlobal>
#include <QGraphicsRectItem>
#include <QScrollBar>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <qpropertyanimation.h>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QGraphicsProxyWidget>
#include <QParallelAnimationGroup>
#include <QMessageBox>
#include <qstring.h>
#include "ui_item.h"
#include "ui_titlebar.h"

SortWindow::SortWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SortWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    QTimer *t=new QTimer();
    connect(t,&QTimer::timeout,this,[=](){
        initItems();
    });
    t->setSingleShot(1);
    t->start(100);

    setWindowFlags(Qt::FramelessWindowHint);

    ui->verticalLayout_2->insertWidget(0,titlebar);

    initWidget();

    connect(titlebar->ui->m_closeButton,&QPushButton::clicked,this,[=](){
        on_stopSortingBtn_clicked();
        delete ui;
    });
    this->show();

    connect(ui->retBtn,&QPushButton::clicked,this,[=](){
        this->close();
        MainWindow* m = new MainWindow;
        m->show();
    });
}

SortWindow::~SortWindow()
{
    delete ui;
}
void print(QVector<RectProxyWidget*>rectProxyItems){
    for(int i = 0;i<rectProxyItems.size();i++){
        qDebug() << i<< ":"<<rectProxyItems[i]->item->getValue()<<" ";
    }
    qDebug()<< endl;
}
void SortWindow::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

}




void SortWindow::unable()
{
    ui->shuffleBtn->setEnabled(0);
    ui->itemsNum->setEnabled(0);
    //        ui->aniDuration->setEnabled(0);
    ui->sortingBtn->setEnabled(0);
    ui->sortingtype->setEnabled(0);
    ui->restoreBtn->setEnabled(0);
}

void SortWindow::enable()
{
    ui->shuffleBtn->setEnabled(1);
    ui->itemsNum->setEnabled(1);
    //    ui->aniDuration->setEnabled(1);
    ui->sortingBtn->setEnabled(1);
    ui->sortingtype->setEnabled(1);
    ui->restoreBtn->setEnabled(1);
}

void SortWindow::initItems(QVector<RectProxyWidget* > proxyItems)
{
    for(int i=0;i<proxyItems.size();i++){
        proxyItems[i]->show();
    }
}

void SortWindow::initArrowLabel(QLabel *l,QGraphicsProxyWidget *arrow,QPixmap pix)
{
    l->setAlignment(Qt::AlignHCenter);
    l->setAttribute(Qt::WA_TranslucentBackground);

    arrow->setWidget(l);
    arrow->setZValue(1);

    l->setPixmap(pix);
    scene->addItem(arrow);

}

void SortWindow::initNumLabel(QString text,QString style,QPointF pos,QGraphicsProxyWidget *labelNum)
{
    QFont f("微软雅黑",20,QFont::Black);
    QLabel *ln1=new QLabel(text);
    ln1->setStyleSheet(style);

    ln1->setFont(f);
    ln1->setAlignment(Qt::AlignHCenter);
    ln1->setAttribute(Qt::WA_TranslucentBackground);
    ln1->adjustSize();

    labelNum->setWidget(ln1);
    labelNum->setZValue(1);
    labelNum->setPos(pos);

    scene->addItem(labelNum);

}

void SortWindow::arrowPosSet()
{
    if(rectProxyItems.size()==1){
        arrowLabelI->setPos(QPoint(rectProxyItems[0]->x(),rectProxyItems[0]->y()-li->height()));
        arrowLabelJ->setPos(QPoint(rectProxyItems[0]->x(),rectProxyItems[0]->y()-lj->height()-lk->height()));
        arrowLabelK->setPos(QPoint(rectProxyItems[0]->x(),rectProxyItems[0]->y()-lj->height()-lk->height()-lk->height()));
        arrowLabelGuard->setPos(QPoint(rectProxyItems[0]->x(),rectProxyItems[0]->y()-lj->height()-lk->height()-lk->height()-lk->height()));
    }
    else if(rectProxyItems.size()>=2){
        arrowLabelI->setPos(QPoint(rectProxyItems[0]->x(),rectProxyItems[0]->y()-li->height()));
        arrowLabelJ->setPos(QPoint(rectProxyItems[0]->x(),rectProxyItems[0]->y()-lj->height()-lk->height()));
        arrowLabelK->setPos(QPoint(rectProxyItems[1]->x(),rectProxyItems[1]->y()-lj->height()));
        arrowLabelGuard->setPos(QPoint(rectProxyItems[0]->x(),rectProxyItems[0]->y()-lj->height()-lk->height()-lk->height()-lk->height()));

    }
}

void SortWindow::removeItemsCopy()
{
    int count=rectProxyItemsCopy.size();

    for(int ix=0;ix<count;ix++){

        RectProxyWidget *r=rectProxyItemsCopy.at(0);
        rectProxyItemsCopy.removeFirst();
        r->deleteLater();
    }
}

void SortWindow::removeItems()
{

    for(int ix=0,count=rectProxyItems.size();ix<count;ix++){

        RectProxyWidget *r=rectProxyItems.at(ix);
        //rectProxyItems.removeFirst();
        scene->removeItem(r);
        //r->deleteLater();
        Sleep(10);
    }
}

void SortWindow::initItems()
{
    //我们先获得有多少个数据，根据scene的宽度来决定。
    //每个数据宽度是多少?


    viewWidth=ui->graphicsView->width();

    viewHeight=ui->graphicsView->height();

    double rectWidth=viewWidth/itemsNum;

    double intetvalX=(viewWidth-(rectWidth*itemsNum))/itemsNum;

    double space=viewHeight/itemsNum;

    double rectHeight;

    qDebug()<<viewHeight<<viewWidth;

    li->setFixedWidth(rectWidth);
    lj->setFixedWidth(rectWidth);
    lk->setFixedWidth(rectWidth);
    lGuard->setFixedWidth(rectWidth);

    //          scene->setSceneRect(0,0,viewWidth,viewHeight);

    Item::setThreshold(itemsNum/2);

    for(int i=0;i<itemsNum;i++){
        rectHeight=space*i;
        Item *rectItem=new Item();
        rectItem->setStandard(i);
        RectProxyWidget *p=new RectProxyWidget(rectItem);

        p->setWidget(rectItem);
        rectItem->setId(i);
        rectItem->setValue(i);
        p->setGeometry(QRectF((intetvalX+rectWidth)*i,viewHeight-rectHeight,rectWidth,rectHeight));
        rectProxyItems.append(p);
    }


    shuffle();
    arrowPosSet();
    for(int i=0;i<itemsNum;i++){
        scene->addItem(rectProxyItems.at(i));
    }


}

void SortWindow::shuffle()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    for(int i=0;i<itemsNum;i++){
        int r=qrand()%itemsNum;
        if(r==i){
            continue;
        }

        QRectF rectR1=rectProxyItems.at(i)->geometry();
        QRectF rectR2=rectProxyItems.at(r)->geometry();

        rectProxyItems[r]->setGeometry(QRectF(rectR1.x(),rectR2.y(),rectR2.width(),rectR2.height()).toRect());

        int tempid=rectProxyItems[i]->item->getId();

        rectProxyItems[i]->item->setId(rectProxyItems[r]->item->getId());
        rectProxyItems[r]->item->setId(tempid);
        rectProxyItems[i]->setGeometry(QRectF(rectR2.x(),rectR1.y(),rectR1.width(),rectR1.height()).toRect());



        RectProxyWidget *item=rectProxyItems.at(i);
        rectProxyItems[i]=rectProxyItems[r];
        rectProxyItems[r]=item;
    }
    //removeItemsCopy();
    rectProxyItemsCopy.clear();
    int c=rectProxyItems.size();
    for (int i=0;i<c;i++) {

        RectProxyWidget *copy=new RectProxyWidget(rectProxyItems.at(i));
        rectProxyItemsCopy.append(copy);
        //Sleep(10);
    }

}



void SortWindow::swapAni(int i, int j, QGraphicsProxyWidget *iw, QGraphicsProxyWidget *jw)
{
    //0
    aniStop=0;

//    int tempVal=rectProxyItems[i]->item->getValue();
//    rectProxyItems[i]->item->setValue(rectProxyItems[j]->item->getValue());
//    rectProxyItems[j]->item->setValue(tempVal);
    int temp=rectProxyItems[i]->item->getId();
    rectProxyItems[i]->item->setId(rectProxyItems[j]->item->getId());
    rectProxyItems[j]->item->setId(temp);




    QParallelAnimationGroup *aniGroup=new QParallelAnimationGroup;

    QPropertyAnimation *aniI =new QPropertyAnimation(iw,"pos");

    QPropertyAnimation *aniJ =new QPropertyAnimation(jw,"pos");

    QPropertyAnimation *ani =new QPropertyAnimation(rectProxyItems[i],"pos");
    QPropertyAnimation *ani2 =new QPropertyAnimation(rectProxyItems[j],"pos");



    aniI->setStartValue(iw->pos());
    aniI->setEndValue(QPointF(rectProxyItems[i]->geometry().x(),rectProxyItems[j]->geometry().y()-lj->height()));
    aniI->setDuration(durationAni);
    aniI->setEasingCurve(QEasingCurve::OutCirc);


    aniJ->setStartValue(jw->pos());
    aniJ->setEndValue(QPoint(rectProxyItems[j]->geometry().x(),rectProxyItems[i]->geometry().y()-lj->height()));
    aniJ->setDuration(durationAni);
    aniJ->setEasingCurve(QEasingCurve::OutCirc);

    if(i!=j){
        QPoint r1(rectProxyItems[i]->x(),rectProxyItems[i]->y());
        QPoint r2(rectProxyItems[j]->x(),rectProxyItems[i]->y());

        ani->setStartValue(r1);
        ani->setEndValue(r2);
        ani->setDuration(durationAni);

        ani->setEasingCurve(QEasingCurve::OutCirc);

        connect(ani,&QPropertyAnimation::finished,this,[=](){
            rectProxyItems[i]->setPos(r2);
        });


        r1=QPoint(rectProxyItems[j]->geometry().x(),rectProxyItems[j]->geometry().y());
        r2=QPoint(rectProxyItems[i]->geometry().x(),rectProxyItems[j]->geometry().y());


        ani2->setStartValue(r1);
        ani2->setEndValue(r2);
        ani2->setDuration(durationAni);

        ani2->setEasingCurve(QEasingCurve::OutCirc);

        connect(ani2,&QPropertyAnimation::finished,this,[=](){
            rectProxyItems[j]->setPos(r2);
        });

        aniGroup->addAnimation(ani);
        aniGroup->addAnimation(ani2);
    }
    else{
        QPoint r1(rectProxyItems[i]->x(),rectProxyItems[i]->y());
        QPoint r2(rectProxyItems[j]->x(),rectProxyItems[i]->y());

        rectProxyItems[i]->setPos(r2);

        r1=QPoint(rectProxyItems[j]->geometry().x(),rectProxyItems[j]->geometry().y());
        r2=QPoint(rectProxyItems[i]->geometry().x(),rectProxyItems[j]->geometry().y());
        rectProxyItems[j]->setPos(r2);

    }


    aniGroup->addAnimation(aniI);
    aniGroup->addAnimation(aniJ);

    aniGroup->start(QAbstractAnimation::DeleteWhenStopped);


    connect(aniGroup,&QParallelAnimationGroup::finished,this,[=](){
        aniStop=1;
        RectProxyWidget *item=rectProxyItems[i];
        rectProxyItems[i]=rectProxyItems[j];
        rectProxyItems[j]=item;
    });



    while(!aniStop){
        Sleep(durationAni);
    }

}

void SortWindow::selectSorting()
{
    labelNumI->show();
    labelNumK->hide();
    labelNumGuard->hide();
    arrowLabelK->hide();
    arrowLabelGuard->hide();

    for(int i=0;i<rectProxyItems.size();i++){
        static_cast<QLabel*>(labelNumI->widget())->setText("i="+QString::number(i)+",i<"+QString::number(rectProxyItems.size()));
        static_cast<QLabel*>(labelNumI->widget())->adjustSize();
        widgetSwapAni(arrowLabelI,rectProxyItems[i],1);
        for(int j=i+1;j<=rectProxyItems.size()-1;++totalStepCount,j++){
            static_cast<QLabel*>(labelNumJ->widget())->setText("j="+QString::number(j)+",j<="+QString::number(rectProxyItems.size()-1));
            static_cast<QLabel*>(labelNumJ->widget())->adjustSize();

            widgetSwapAni(arrowLabelJ,rectProxyItems[j],1);
            if(!sorting){
                return;
            }
            if(smallToLarge){
                if(rectProxyItems[i]->item->getValue()>rectProxyItems[j]->item->getValue()){
                    swapAni(i,j,arrowLabelI,arrowLabelJ);
                    totalSwapCount++;
                }
                else{
                    totalCmpCount++;
                }
            }
            else{
                if(rectProxyItems[i]->item->getValue()<rectProxyItems[j]->item->getValue()){
                    swapAni(i,j,arrowLabelI,arrowLabelJ);
                    totalSwapCount++;
                }
                else{
                    totalCmpCount++;
                }
            }

            ui->lcdNumber->display(totalStepCount);
            ui->lcdNumber_2->display(totalCmpCount);
            ui->lcdNumber_3->display(totalSwapCount);

        }
    }
}

void SortWindow::bubbleSorting()
{
    labelNumK->show();
    arrowLabelK->show();
    arrowLabelI->hide();
    arrowLabelGuard->hide();
    for(int i=0;i<rectProxyItems.size()-1;i++){
        static_cast<QLabel*>(labelNumI->widget())->setText("i="+QString::number(i)+",i<"+QString::number(rectProxyItems.size()));
        static_cast<QLabel*>(labelNumI->widget())->adjustSize();
        for(int j=0;j<rectProxyItems.size()-i-1;++totalStepCount,j++){


            static_cast<QLabel*>(labelNumJ->widget())->setText("j="+QString::number(j)+",j<"+QString::number(rectProxyItems.size()-i));
            static_cast<QLabel*>(labelNumJ->widget())->adjustSize();


            static_cast<QLabel*>(labelNumK->widget())->setText("k=j-1="+QString::number(j-1));
            static_cast<QLabel*>(labelNumK->widget())->adjustSize();

            widgetSwapAni(arrowLabelJ,rectProxyItems[j],0);
            widgetSwapAni(arrowLabelK,rectProxyItems[j+1],1);

            if(!sorting){
                return;
            }
            if(smallToLarge){
                if(rectProxyItems[j]->item->getValue()>rectProxyItems[j+1]->item->getValue()){
                    swapAni(j,j+1,arrowLabelK,arrowLabelJ);
                    qDebug()<<"交换后后"<<endl;
                    print(rectProxyItems);
                    totalSwapCount++;
                }
                else{
                    ++totalCmpCount;
                }
            }
            else{
                if(rectProxyItems[j]->item->getValue()<rectProxyItems[j+1]->item->getValue()){
                    swapAni(j,j+1,arrowLabelK,arrowLabelJ);
                    totalSwapCount++;
                }
                else{
                    ++totalCmpCount;
                }
            }
            ui->lcdNumber->display(totalStepCount);
            ui->lcdNumber_2->display(totalCmpCount);
            ui->lcdNumber_3->display(totalSwapCount);
        }
    }

}

void SortWindow::bubbleSortingEx()
{
    labelNumK->show();
    arrowLabelK->show();
    arrowLabelI->hide();
    arrowLabelGuard->hide();


    for(int i=0;i<rectProxyItems.size()-1;i++){
        bool isSorted=1;
        static_cast<QLabel*>(labelNumI->widget())->setText("i="+QString::number(i)+",i<"+QString::number(rectProxyItems.size()));
        static_cast<QLabel*>(labelNumI->widget())->adjustSize();

        for(int j=0;j<rectProxyItems.size()-i-1;++totalStepCount,j++){


            static_cast<QLabel*>(labelNumJ->widget())->setText("j="+QString::number(j)+",j<"+QString::number(rectProxyItems.size()-i));
            static_cast<QLabel*>(labelNumJ->widget())->adjustSize();

            static_cast<QLabel*>(labelNumK->widget())->setText("k=j-1="+QString::number(j-1));
            static_cast<QLabel*>(labelNumK->widget())->adjustSize();

            widgetSwapAni(arrowLabelJ,rectProxyItems[j],0);
            widgetSwapAni(arrowLabelK,rectProxyItems[j+1],1);

            if(!sorting){
                return;
            }
            if(smallToLarge){
                if(rectProxyItems[j]->item->getValue()>rectProxyItems[j+1]->item->getValue()){
                    swapAni(j,j+1,arrowLabelK,arrowLabelJ);
                    totalSwapCount++;
                    isSorted = 0;
                }
                else{
                    ++totalCmpCount;
                }
            }
            else{
                if(rectProxyItems[j]->item->getValue()<rectProxyItems[j+1]->item->getValue()){
                    swapAni(j,j+1,arrowLabelK,arrowLabelJ);
                    totalSwapCount++;
                    isSorted = 0;
                }
                else{
                    ++totalCmpCount;
                }
            }

            ui->lcdNumber->display(totalStepCount);
            ui->lcdNumber_2->display(totalCmpCount);
            ui->lcdNumber_3->display(totalSwapCount);

        }
        if(isSorted)
            break;
    }
}

void SortWindow::insertSorting()
{

    labelNumK->show();
    arrowLabelK->show();
    arrowLabelI->hide();
    for(int i=1;i<rectProxyItems.size();i++){
        static_cast<QLabel*>(labelNumI->widget())->setText("i="+QString::number(i)+",i<"+QString::number(rectProxyItems.size()));
        static_cast<QLabel*>(labelNumI->widget())->adjustSize();
        for(int j=i;j>0;++totalStepCount,--j){

            static_cast<QLabel*>(labelNumJ->widget())->setText("j="+QString::number(j)+",j<"+QString::number(rectProxyItems.size()-i));
            static_cast<QLabel*>(labelNumJ->widget())->adjustSize();
            static_cast<QLabel*>(labelNumK->widget())->setText("k=j-1="+QString::number(j-1));
            static_cast<QLabel*>(labelNumK->widget())->adjustSize();

            widgetSwapAni(arrowLabelJ,rectProxyItems[j],0);
            widgetSwapAni(arrowLabelK,rectProxyItems[j-1],1);

            if(!sorting){
                return;
            }
            if(smallToLarge){
                if(rectProxyItems[j]->item->getValue()<rectProxyItems[j-1]->item->getValue()){
                    swapAni(j,j-1,arrowLabelK,arrowLabelJ);
                    totalSwapCount++;
                }
                else{
                    ++totalCmpCount;
                }
            }
            else{
                if(rectProxyItems[j]->item->getValue()>rectProxyItems[j-1]->item->getValue()){
                    swapAni(j,j-1,arrowLabelK,arrowLabelJ);
                    totalSwapCount++;
                }
                else{
                    ++totalCmpCount;
                }
            }
            ui->lcdNumber->display(totalStepCount);
            ui->lcdNumber_2->display(totalCmpCount);
            ui->lcdNumber_3->display(totalSwapCount);
        }
    }
}

void SortWindow::insertSortingEx()
{
    labelNumK->show();
    arrowLabelK->show();
    arrowLabelI->hide();
    arrowLabelGuard->show();
    int i,j;
    for(i=1;i<rectProxyItems.size();i++){
        static_cast<QLabel*>(labelNumI->widget())->setText("i="+QString::number(i)+",i<"+QString::number(rectProxyItems.size()));
        static_cast<QLabel*>(labelNumI->widget())->adjustSize();
        if(smallToLarge){
            if( rectProxyItems[i]->item->getValue()<rectProxyItems[i-1]->item->getValue())
            {
                //temp是哨兵
                int temp=rectProxyItems[i]->item->getValue();
                static_cast<QLabel*>(labelNumGuard->widget())->setText("g="+QString::number(temp));
                static_cast<QLabel*>(labelNumGuard->widget())->adjustSize();
                widgetSwapAni(arrowLabelGuard,rectProxyItems[i],1);
                for(j=i-1;j>=0;++totalStepCount,j--)
                {
                    if(rectProxyItems[j]->item->getValue()>temp){
                        static_cast<QLabel*>(labelNumJ->widget())->setText("j="+QString::number(j)+",j<"+QString::number(rectProxyItems.size()-i));
                        static_cast<QLabel*>(labelNumJ->widget())->adjustSize();
                        static_cast<QLabel*>(labelNumK->widget())->setText("k=j-1="+QString::number(j-1));
                        static_cast<QLabel*>(labelNumK->widget())->adjustSize();
                        widgetSwapAni(arrowLabelJ,rectProxyItems[j],0);
                        widgetSwapAni(arrowLabelK,rectProxyItems[j+1],1);
                        if(!sorting){
                            return;
                        }
                        swapAni(j+1,j,arrowLabelK,arrowLabelJ);
                        widgetSwapAni(arrowLabelGuard,rectProxyItems[j+1],0);
                        totalSwapCount++;
                    }
                    else{
                        ++totalCmpCount;
                        break;
                    }
                    ui->lcdNumber->display(totalStepCount);
                    ui->lcdNumber_2->display(totalCmpCount);
                    ui->lcdNumber_3->display(totalSwapCount);
                }
                rectProxyItems[j+1]->item->setValue(temp);
            }
            else{
                ++totalCmpCount;
            }
        }
        else{
            if( rectProxyItems[i]->item->getValue()>rectProxyItems[i-1]->item->getValue())
            {
                //temp是哨兵
                int temp=rectProxyItems[i]->item->getValue();
                static_cast<QLabel*>(labelNumGuard->widget())->setText("g="+QString::number(temp));
                static_cast<QLabel*>(labelNumGuard->widget())->adjustSize();
                widgetSwapAni(arrowLabelGuard,rectProxyItems[i],1);
                for(j=i-1;j>=0;++totalStepCount,j--)
                {
                    if(rectProxyItems[j]->item->getValue()<temp){
                        static_cast<QLabel*>(labelNumJ->widget())->setText("j="+QString::number(j)+",j<"+QString::number(rectProxyItems.size()-i));
                        static_cast<QLabel*>(labelNumJ->widget())->adjustSize();

                        static_cast<QLabel*>(labelNumK->widget())->setText("k=j-1="+QString::number(j-1));
                        static_cast<QLabel*>(labelNumK->widget())->adjustSize();
                        widgetSwapAni(arrowLabelJ,rectProxyItems[j],0);
                        widgetSwapAni(arrowLabelK,rectProxyItems[j+1],1);
                        if(!sorting){
                            return;
                        }
                        swapAni(j+1,j,arrowLabelK,arrowLabelJ);
                        arrowLabelGuard->setPos(arrowLabelK->pos());
                        totalSwapCount++;
                    }
                    else{
                        ++totalCmpCount;
                        break;
                    }
                    ui->lcdNumber->display(totalStepCount);
                    ui->lcdNumber_2->display(totalCmpCount);
                    ui->lcdNumber_3->display(totalSwapCount);
                }
                rectProxyItems[j+1]->item->setValue(temp);
            }
            else{
                ++totalCmpCount;
            }
        }


    }
}

void SortWindow::quickSorting(int low,int high)
{
    if (!sorting) {
        return;
    }
    //qDebug() << "begin" << endl;
    int privotLoc = partition(low, high); //将表一分为二
    //qDebug() <<"privotLoc" << privotLoc << endl;
    if (low < privotLoc - 1)//递归对低子表递归排序
    {
        quickSorting(low, privotLoc - 1);
    }
    if (privotLoc + 1 < high)//递归对高子表递归排序
    {
        quickSorting(privotLoc + 1, high);
    }
    //qDebug() << "end" << endl;

}

void SortWindow::quickSortingEx(int low, int high)
{
    if (!sorting) {
        return;
    }
    int mid = (low + high) / 2;
    int lowValue  = rectProxyItems[low]->item->getValue();
    int midValue  = rectProxyItems[mid]->item->getValue();
    int highValue = rectProxyItems[high]->item->getValue();

    if ((midValue - lowValue) * (lowValue-highValue) >= 0)
    {
        totalCmpCount ++;
    }
    else if ((lowValue - midValue)*(midValue - highValue) >= 0)
    {
        swapAni(low, mid, arrowLabelJ, arrowLabelK);
        totalCmpCount+=2;
        totalSwapCount++;
    }
    else
    {
        swapAni(low, high, arrowLabelJ, arrowLabelK);
        totalCmpCount += 2;
        totalSwapCount++;
    }
    int privotLoc = partition(low, high); //将表一分为二
    if (low < privotLoc - 1)//递归对低子表递归排序
    {
        quickSorting(low, privotLoc - 1);
    }
    if (privotLoc + 1 < high)//递归对高子表递归排序
    {
        quickSorting(privotLoc + 1, high);
    }
}

int SortWindow::partition(int low, int high)
{
    if (low == high)
    {
        return low;
    }
    int privotKey = rectProxyItems[low]->item->getValue();               //基准元素
  //  qDebug() << "privotKey" << privotKey << endl;
    int guardPos=low;
    static_cast<QLabel*>(labelNumGuard->widget())->setText("privotKey="+QString::number(privotKey));
    static_cast<QLabel*>(labelNumGuard->widget())->adjustSize();

    widgetSwapAni(arrowLabelGuard,rectProxyItems[guardPos],1);

    while(low < high){                  //从表的两端交替地向中间扫描

        if(smallToLarge){
            while(low < high && rectProxyItems[high]->item->getValue() >= privotKey) {
                if (!sorting) {
                    return -1;
                }
                totalStepCount++;
                --high; //从high 所指位置向前搜索，至多到low+1 位置。将比基准元素小的交换到低端
                static_cast<QLabel*>(labelNumK->widget())->setText("high="+QString::number(high));
                static_cast<QLabel*>(labelNumK->widget())->adjustSize();
                widgetSwapAni(arrowLabelK,rectProxyItems[high],1);

                totalCmpCount+=2;
                totalSwapCount++;

                ui->lcdNumber->display(totalStepCount);
                ui->lcdNumber_2->display(totalCmpCount);
                ui->lcdNumber_3->display(totalSwapCount);

            }


            while(low < high && rectProxyItems[low]->item->getValue()  <= privotKey ) {
                if (!sorting) {
                    return -1;
                }
                totalStepCount++;
                ++low;
                static_cast<QLabel*>(labelNumJ->widget())->setText("low="+QString::number(low)+",low<high="+QString::number(high));
                static_cast<QLabel*>(labelNumJ->widget())->adjustSize();
                widgetSwapAni(arrowLabelJ,rectProxyItems[low],1);

                totalCmpCount+=2;
                totalSwapCount++;

                ui->lcdNumber->display(totalStepCount);
                ui->lcdNumber_2->display(totalCmpCount);
                ui->lcdNumber_3->display(totalSwapCount);

            }
            if (low < high) {
                swapAni(low, high, arrowLabelJ, arrowLabelK);
                widgetSwapAni(arrowLabelJ, rectProxyItems[low], 1);
                widgetSwapAni(arrowLabelK, rectProxyItems[high], 1);
                ui->lcdNumber->display(totalStepCount);
                ui->lcdNumber_2->display(totalCmpCount);
                ui->lcdNumber_3->display(totalSwapCount);
            }
        }
        else{
            while(low < high && rectProxyItems[high]->item->getValue() < privotKey) {
                if (!sorting) {
                    return -1;
                }
                totalStepCount++;
                --high; //从high 所指位置向前搜索，至多到low+1 位置。将比基准元素小的交换到低端
                static_cast<QLabel*>(labelNumK->widget())->setText("high="+QString::number(high));
                static_cast<QLabel*>(labelNumK->widget())->adjustSize();

                widgetSwapAni(arrowLabelK,rectProxyItems[high],1);

                totalCmpCount+=2;
                totalSwapCount++;

                ui->lcdNumber->display(totalStepCount);
                ui->lcdNumber_2->display(totalCmpCount);
                ui->lcdNumber_3->display(totalSwapCount);

            }
            widgetSwapAni(arrowLabelGuard,rectProxyItems[guardPos],1);

            while(low < high && rectProxyItems[low]->item->getValue()  > privotKey ) {
                if (!sorting) {
                    return -1;
                }
                totalStepCount++;
                ++low;
                static_cast<QLabel*>(labelNumJ->widget())->setText("low="+QString::number(low)+",low<high="+QString::number(high));
                static_cast<QLabel*>(labelNumJ->widget())->adjustSize();
                widgetSwapAni(arrowLabelJ,rectProxyItems[low],1);

                totalCmpCount+=2;
                totalSwapCount++;

                ui->lcdNumber->display(totalStepCount);
                ui->lcdNumber_2->display(totalCmpCount);
                ui->lcdNumber_3->display(totalSwapCount);

            }
            swapAni(low,high,arrowLabelJ,arrowLabelK);
            widgetSwapAni(arrowLabelGuard,rectProxyItems[guardPos],1);


        }
    }
    totalStepCount++;
    totalCmpCount++;
    swapAni(low, guardPos, arrowLabelJ, arrowLabelK);
    widgetSwapAni(arrowLabelJ, rectProxyItems[low], 1);
    widgetSwapAni(arrowLabelK, rectProxyItems[guardPos], 1);

    ui->lcdNumber->display(totalStepCount);
    ui->lcdNumber_2->display(totalCmpCount);
    ui->lcdNumber_3->display(totalSwapCount);
    return low;
}

void SortWindow::heapSorting()
{
    arrowLabelI->show();
    arrowLabelJ->show();
    arrowLabelK->hide();
    arrowLabelGuard->hide();
    labelNumGuard->hide();
    labelNumK->hide();
    for(int i=rectProxyItems.size()/2 - 1; i >= 0; i--)  // 对每一个非叶结点进行堆调整(从最后一个非叶结点开始)
    {
        if(!sorting){
            return;
        }
        static_cast<QLabel*>(labelNumI->widget())->setText("i="+QString::number(i)+",i<"+QString::number(rectProxyItems.size()/2-1));
        static_cast<QLabel*>(labelNumI->widget())->adjustSize();
        adjust(rectProxyItems, rectProxyItems.size(), i);
    }
    for(int i = rectProxyItems.size() - 1; i >= 1; i--)
    {
        if(!sorting){
            return;
        }
        static_cast<QLabel*>(labelNumI->widget())->setText("i="+QString::number(i)+",i<"+QString::number(rectProxyItems.size()-1));
        static_cast<QLabel*>(labelNumI->widget())->adjustSize();
        widgetSwapAni(arrowLabelI,rectProxyItems[0],0);
        widgetSwapAni(arrowLabelJ,rectProxyItems[i],1);
        swapAni(0,i,arrowLabelJ,arrowLabelI);
        //swap(rectProxyItems,0, i);           // 将当前最大的放置到数组末尾
        adjust(rectProxyItems, i, 0);              // 将未完成排序的部分继续进行堆排序
    }
}


void SortWindow::adjust(QVector<RectProxyWidget*>rectProxyItems, int len, int index)
{
    if(smallToLarge){
        while(true){
            totalStepCount++;
            int left = 2*index + 1;
            if(left>=len){
                break;
            }
            int right = 2*index + 2;
            int maxIdx = index;
            if(left<len){
                widgetSwapAni(arrowLabelJ,rectProxyItems[left],0);
                widgetSwapAni(arrowLabelI,rectProxyItems[index],1);
                if(!sorting){
                    return;
                }
                static_cast<QLabel*>(labelNumJ->widget())->setText("j="+QString::number(left)+",j<"+QString::number(len));
                static_cast<QLabel*>(labelNumJ->widget())->adjustSize();
            }
            totalCmpCount++;
            if(left<len && rectProxyItems[left]->item->getValue() > rectProxyItems[maxIdx]->item->getValue()) {
                maxIdx = left;
            }
            totalCmpCount++;
            if(right<len && rectProxyItems[right]->item->getValue() > rectProxyItems[maxIdx]->item->getValue()){
                maxIdx = right;
                if(!sorting){
                    return;
                }
                static_cast<QLabel*>(labelNumJ->widget())->setText("j="+QString::number(right)+",j<"+QString::number(len));
                static_cast<QLabel*>(labelNumJ->widget())->adjustSize();
                if(!sorting){
                    return;
                }
                widgetSwapAni(arrowLabelJ,rectProxyItems[right],1);
            }
            if(maxIdx != index)                 // 如果maxIdx的值有更新
            {
                totalSwapCount++;
                if(!sorting){
                    return;
                }
                swapAni(index,maxIdx,arrowLabelJ,arrowLabelI);
                if(!sorting){
                    return;
                }
                RectProxyWidget *item=rectProxyItems[index];
                rectProxyItems[index]=rectProxyItems[maxIdx];
                rectProxyItems[maxIdx]=item;

                index = maxIdx;
                ui->lcdNumber->display(totalStepCount);
                ui->lcdNumber_2->display(totalCmpCount);
                ui->lcdNumber_3->display(totalSwapCount);
            }else{
                ui->lcdNumber->display(totalStepCount);
                ui->lcdNumber_2->display(totalCmpCount);
                ui->lcdNumber_3->display(totalSwapCount);
                break;
            }
        }
    }else{
        while(true){
            if(!sorting){
                return;
            }
            totalStepCount++;
            int left = 2*index + 1;
            if(left>=len){
                break;
            }
            int right = 2*index + 2;
            int maxIdx = index;
            if(left<len){
                widgetSwapAni(arrowLabelJ,rectProxyItems[left],0);
                widgetSwapAni(arrowLabelI,rectProxyItems[index],1);
                if(!sorting){
                    return;
                }
                static_cast<QLabel*>(labelNumJ->widget())->setText("j="+QString::number(left)+",j<"+QString::number(len));
                static_cast<QLabel*>(labelNumJ->widget())->adjustSize();
            }
            totalCmpCount++;
            if(left<len && rectProxyItems[left]->item->getValue() < rectProxyItems[maxIdx]->item->getValue()) {
                maxIdx = left;
            }
            totalCmpCount++;
            if(right<len && rectProxyItems[right]->item->getValue() < rectProxyItems[maxIdx]->item->getValue()){
                maxIdx = right;
                if(!sorting){
                    return;
                }
                static_cast<QLabel*>(labelNumJ->widget())->setText("j="+QString::number(right)+",j<"+QString::number(len));
                static_cast<QLabel*>(labelNumJ->widget())->adjustSize();
                if(!sorting){
                    return;
                }
                widgetSwapAni(arrowLabelJ,rectProxyItems[right],1);
            }
            if(maxIdx != index)                 // 如果maxIdx的值有更新
            {
                totalSwapCount++;
                if(!sorting){
                    return;
                }
                swapAni(index,maxIdx,arrowLabelJ,arrowLabelI);
                if(!sorting){
                    return;
                }
                RectProxyWidget *item=rectProxyItems[index];
                rectProxyItems[index]=rectProxyItems[maxIdx];
                rectProxyItems[maxIdx]=item;

                index = maxIdx;
                ui->lcdNumber->display(totalStepCount);
                ui->lcdNumber_2->display(totalCmpCount);
                ui->lcdNumber_3->display(totalSwapCount);
            }else{
                ui->lcdNumber->display(totalStepCount);
                ui->lcdNumber_2->display(totalCmpCount);
                ui->lcdNumber_3->display(totalSwapCount);
                break;
            }
        }
    }


}


void SortWindow::bucketSort(QVector<RectProxyWidget*> &bucket){
    labelNumK->show();
    arrowLabelK->show();
    arrowLabelI->hide();
    arrowLabelGuard->hide();
    for(int i=0;i<rectProxyItems.size()-1;i++){
        static_cast<QLabel*>(labelNumI->widget())->setText("i="+QString::number(i)+",i<"+QString::number(rectProxyItems.size()));
        static_cast<QLabel*>(labelNumI->widget())->adjustSize();
        for(int j=0;j<rectProxyItems.size()-i-1;++totalStepCount,j++){


            static_cast<QLabel*>(labelNumJ->widget())->setText("j="+QString::number(j)+",j<"+QString::number(rectProxyItems.size()-i));
            static_cast<QLabel*>(labelNumJ->widget())->adjustSize();


            static_cast<QLabel*>(labelNumK->widget())->setText("k=j-1="+QString::number(j-1));
            static_cast<QLabel*>(labelNumK->widget())->adjustSize();

            widgetSwapAni(arrowLabelJ,rectProxyItems[j],0);
            widgetSwapAni(arrowLabelK,rectProxyItems[j+1],1);

            if(!sorting){
                return;
            }
            if(smallToLarge){
                if(rectProxyItems[j]->item->getValue()>rectProxyItems[j+1]->item->getValue()){
                    swapAni(j,j+1,arrowLabelK,arrowLabelJ);
                    RectProxyWidget* temp  = bucket[j];
                    bucket[j] = bucket[j+1];
                    bucket[j+1] = temp;
                    totalSwapCount++;
                }
                else{
                    ++totalCmpCount;
                }
            }
            else{
                if(rectProxyItems[j]->item->getValue()<rectProxyItems[j+1]->item->getValue()){
                    swapAni(j,j+1,arrowLabelK,arrowLabelJ);
                    RectProxyWidget* temp  = bucket[j];
                    bucket[j] = bucket[j+1];
                    bucket[j+1] = temp;
                    totalSwapCount++;
                }
                else{
                    ++totalCmpCount;
                }
            }
            ui->lcdNumber->display(totalStepCount);
            ui->lcdNumber_2->display(totalCmpCount);
            ui->lcdNumber_3->display(totalSwapCount);
        }
    }
}

void SortWindow::bucketSorting()
{
    arrowLabelI->show();
    arrowLabelJ->show();
    arrowLabelK->hide();
    arrowLabelGuard->hide();
    labelNumI->show();
    labelNumJ->show();
    labelNumK->hide();
    labelNumGuard->hide();


    int i ;
    QVector<QVector<RectProxyWidget*>> bucket(100);

    for (i = 0;i < rectProxyItems.size();i++)
        bucket[rectProxyItems[i]->item->getValue() / 10].push_back(rectProxyItems[i]);

    for(int i = 0;i<bucket.size();i++){
        rectProxyItems = bucket[i];
        bucketSort(bucket[i]);
    }



    QVector<RectProxyWidget*> rectProxyItemsTemp;
    for(int i = 0;i<bucket.size();i++){
        for(int j = 0;j<bucket[i].size();j++){
            rectProxyItemsTemp.push_back(bucket[i][j]);
        }
    }
    rectProxyItems = rectProxyItemsTemp;
    return;
}

void SortWindow::merge(int start1, int end1, int start2, int end2)
{

    int i = start1, j = start2;
    int n = (end1 - start1 + 1) + (end2 - start2 + 1);
    int* temp = new int[n];
    int k = 0;
    while (i <= end1 && j <= end2) {
        if (rectProxyItems[i]->item->getValue() < rectProxyItems[j]->item->getValue())
            temp[k++] = rectProxyItems[i++]->item->getValue();
        else
            temp[k++] = rectProxyItems[j++]->item->getValue();
    }
    while (i <= end1)temp[k++] = rectProxyItems[i++]->item->getValue();
    while (j <= end2)temp[k++] = rectProxyItems[j++]->item->getValue();

    for(i=0;i<n;i++){
        for(j=start1;j<rectProxyItems.size();j++){
            if(rectProxyItems[j]->item->getValue()==temp[i]){
                widgetSwapAni(arrowLabelI,rectProxyItems[i+start1],0);
                widgetSwapAni(arrowLabelJ,rectProxyItems[j],1);
                swapAni(i+start1, j, arrowLabelJ,arrowLabelI);
                totalSwapCount++;
                break;
            }
        }
    }

    ui->lcdNumber->display(totalStepCount);
    ui->lcdNumber_2->display(totalCmpCount);
    ui->lcdNumber_3->display(totalSwapCount);
}

void SortWindow::mergeSorting(int start,int end)
{
    arrowLabelI->show();
    arrowLabelJ->show();
    arrowLabelK->hide();
    arrowLabelGuard->hide();
    labelNumI->show();
    labelNumJ->show();
    labelNumK->hide();
    labelNumGuard->hide();

    if (start < end) {
        int mid = (start + end) / 2;
        mergeSorting(start, mid);
        mergeSorting(mid + 1, end);
        merge(start, mid, mid + 1, end);
    }
}

void SortWindow::widgetSwapAni(QGraphicsProxyWidget *w, RectProxyWidget *w2, bool wait)
{
    aniStop=0;
    QPropertyAnimation *aniW =new QPropertyAnimation(w,"pos");

    aniW->setStartValue(w->pos());
    aniW->setEndValue(QPointF(w2->geometry().x(),w2->geometry().y()-w->rect().height()));
    aniW->setDuration(durationAni);
    aniW->setEasingCurve(QEasingCurve::OutCirc);

    aniW->start(QAbstractAnimation::DeleteWhenStopped);

    connect(aniW,&QPropertyAnimation::finished,this,[=](){
        aniStop=1;

    });
    if(wait)
    while(!aniStop){
        Sleep(durationAni);
    }
}

void SortWindow::initWidget()
{
    durationAni=ui->aniDuration->text().toInt();
    itemsToNum=ui->itemsNum->text().toInt();

    initArrowLabel(li,arrowLabelI,QPixmap(":/箭头.svg"));
    initArrowLabel(lj,arrowLabelJ,QPixmap(":/箭头2.svg"));
    initArrowLabel(lk,arrowLabelK,QPixmap(":/箭头3.svg"));
    initArrowLabel(lGuard,arrowLabelGuard,QPixmap(":/哨兵_2.svg"));

    arrowLabelGuard->hide();

    initNumLabel("i=0,","color: rgb(212, 35, 122)",QPointF(0,0),labelNumI);
    initNumLabel("j=0,","color: rgb(18,150,219)",labelNumI->geometry().bottomLeft(),labelNumJ);
    initNumLabel("k=0,","color: rgb(19,34,122)",labelNumJ->geometry().bottomLeft(),labelNumK);
    initNumLabel("g=0","color: rgb(0,0,0)",labelNumK->geometry().bottomLeft(),labelNumGuard);
}

void SortWindow::on_sortingBtn_clicked()
{
    unable();
    sorting=1;
    switch (ui->sortingtype->currentIndex()) {
    case SELECT_SORTING:
        selectSorting();
        break;
    case BUBBLE_SORTING:
        bubbleSorting();
        break;
    case INSERT_SORTING:
        insertSorting();
        break;
    case BUBBLE_EX_SORTING:

        bubbleSortingEx();
        break;
    case INSERT_EX_SORTING:
        insertSortingEx();
        break;
    case QUICK_SORTING:
        arrowLabelI->hide();
        arrowLabelGuard->show();
        quickSorting(0,rectProxyItems.size()-1);
        break;
    case HEAP_SORT:
        arrowLabelI->hide();
        arrowLabelGuard->show();
        heapSorting();
        break;
    case BUCKET_SORT:
        arrowLabelI->hide();
        arrowLabelGuard->show();
        bucketSorting();
        break;
    case QUICK_EX_SORTING:
        arrowLabelI->hide();
        arrowLabelGuard->show();
        quickSortingEx(0,rectProxyItems.size()-1);
        break;
    case MERGE_SORT:
        arrowLabelI->hide();
        arrowLabelGuard->show();
        mergeSorting(0,rectProxyItems.size()-1);
        break;
    default:
        break;
    }
    enable();

}

void SortWindow::on_shuffleBtn_clicked()
{
    totalStepCount=0;
    totalCmpCount=0;
    totalSwapCount=0;

    shuffle();
    arrowPosSet();
}


void SortWindow::on_stopSortingBtn_clicked()
{
    sorting=0;
    aniStop=1;
}

void SortWindow::on_itemsNum_textChanged(const QString &arg1)
{
    itemsToNum=ui->itemsNum->text().toInt();
    if(itemsToNum>0){
        removeItems();
        rectProxyItems.clear();
        itemsNum=itemsToNum;

        initItems();
    }

    else{
        QMessageBox::warning(this,"一定要大于0","一定要大于0",QMessageBox::NoButton);
    }
}

void SortWindow::on_aniDuration_textChanged(const QString &arg1)
{
    if(ui->aniDuration->text().toInt()>=1)
        durationAni=ui->aniDuration->text().toInt();
    else{
        QMessageBox::warning(this,"一定要>=1","一定要大于>=1",QMessageBox::NoButton);
    }
}

void SortWindow::on_sortingtype_currentIndexChanged(int index)
{

    switch (index) {
    case SELECT_SORTING:

        break;
    case BUBBLE_SORTING:

        break;
    case INSERT_SORTING:

        break;
    default:
        break;
    }
    totalStepCount=0;
    totalCmpCount=0;
    totalSwapCount=0;
}

void SortWindow::on_restoreBtn_clicked()
{
    totalStepCount=0;
    totalCmpCount=0;
    totalSwapCount=0;

    removeItems();
    rectProxyItems.clear();
    int c=rectProxyItemsCopy.size();

    for (int i=0;i<c;i++) {
        RectProxyWidget *copy=new RectProxyWidget(rectProxyItemsCopy.at(i));
        copy->show();
        scene->addItem(copy);
        Sleep(10);
        rectProxyItems.append(copy);

    }

    arrowPosSet();


}

void SortWindow::on_checkBox_clicked(bool checked)
{
    smallToLarge=checked;
}
