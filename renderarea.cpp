#include "renderarea.h"
#include <QApplication>
#include <QPainter>
#include <qglobal.h>
#include <QTime>
#include <QPen>

RenderArea::RenderArea(BinarySearchTree<int> *bst, QWidget *parent) : QWidget(parent), bst(),
    scale(1.0)
{
    this->bst = bst;
    this->scale = 1;
    this->nodeColor = Qt::red;
    this->backgroundColor = Qt::white;
    this->textColor = Qt::black;
}


QSize RenderArea::sizeHint() const
{
    return QSize(50, 50);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(50, 50);
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{

    if (this->bst->isEmpty())
        return;
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    QBrush brush;
    brush.setColor(this->nodeColor);
    brush.setStyle(Qt::SolidPattern);
    QPen pen;
    pen.setColor(this->textColor);

    painter.setBrush(brush);
    painter.setPen(pen);

    this->bst->draw(&painter, this->scale);
    this->autoSize();
}

void RenderArea::changeNodeColor(QColor c)
{
    this->nodeColor = c;
}

void RenderArea::changeBackgroundColor(QColor c)
{
    this->backgroundColor = c;
    QString style("background-color: ");
    style.append(c.name());
    style.append(";");
    this->setStyleSheet(style);
}

void RenderArea::changeTextColor(QColor c)
{
    this->textColor = c;
}

QColor RenderArea::getNodeColor() const
{
    return this->nodeColor;
}

QColor RenderArea::getBackgroundColor() const
{
    return this->backgroundColor;
}

QColor RenderArea::getTextColor() const
{
    return this->textColor;
}

void RenderArea::callRepaint()
{
    if (this->bst->isEmpty())
        return;

    this->scale += 0.1;
    this->repaint();
    this->scale -= 0.1;
    this->repaint();
}

void RenderArea::zoomIn() {
    if (this->bst->isEmpty())
        return;
    if(this->scale < 2.0){
        this->scale += 0.1;
        this->repaint();
    }
}

void RenderArea::zoomOut() {
    if (this->bst->isEmpty())
        return;
    if(this->scale > 0.2) {
        this->scale -= 0.1;
        this->repaint();
    }
}

bool RenderArea::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mb = static_cast<QMouseEvent *>(event);
        switch(mb->button()){
        case Qt::LeftButton:
            this->zoomIn();
            break;
        case Qt::RightButton:
            this->zoomOut();
            break;
        default:
            return true;
        }
    }

    return true;
}

void RenderArea::autoSize() {
    QSize size(bst->getTotalX(), bst->getTotalY());
    this->setMinimumSize(size);
    this->resize(size);
}

void RenderArea::mouseReleaseEvent(QMouseEvent *event)
{
    switch(event->button()){
    case Qt::LeftButton:
        if ( event->modifiers() & Qt::ControlModifier )
        {
            this->bst->deleteAtLocation(event->x(), event->y());
            this->repaint();
            break;
        }

        this->zoomIn();

        break;
    case Qt::RightButton:
        this->zoomOut();
        break;
    default:
        return;
    }
}
