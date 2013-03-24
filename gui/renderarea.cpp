#include <QtGui>
#include <QDebug>

#include <iostream>

#include "renderarea.h"
#include "noximguimanager.h"


RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_PaintOutsidePaintEvent,true);
    painter = NULL;

    shape = Polygon;
    antialiased = false;
    transformed = false;

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    NoximGuiManager* gui = NoximGuiManager::getInstance();
    connect(gui, SIGNAL(hexagonCreated(int)),
            this, SLOT(drawHexagon(int)));
}
QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}
QSize RenderArea::sizeHint() const
{
    return QSize(600, 400);
}
void RenderArea::setShape(Shape shape)
{
    this->shape = shape;
    update();
}
void RenderArea::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}
void RenderArea::setBrush(const QBrush &brush)
{
    this->brush = brush;
    update();
}
void RenderArea::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}
void RenderArea::setTransformed(bool transformed)
{
    this->transformed = transformed;
    update();
}

int c;
void RenderArea::drawHexagon(int count)
{
    std::cout << "-------------- drawHexagon------------------,count = " << count << std::endl;

    if(painter==NULL)
        painter = new QPainter(this);

    painter->setPen(pen);
    painter->setBrush(brush);
    if (antialiased)
        painter->setRenderHint(QPainter::Antialiasing, true);

    painter->translate(50, 50);
    painter->scale(30, 30);

    count--;

    // draw polygon
    static const QPointF points[6] = {
        QPointF(-0.5, 0.866+count),
        QPointF(0.5, 0.866+count),
        QPointF(1, 0+count),
        QPointF(0.5, -0.866+count),
        QPointF(-0.5, -0.866+count),
        QPointF(-1,0+count)
    };
    this->painter->drawPolygon(points, 6);

}



void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    std::cout << "-------------- RenderArea::paintEvent------------------" << std::endl;

//    QPainter painter(this);
    if(painter==NULL)
        painter = new QPainter(this);


//    painter->translate(50, 50);
//    painter->scale(30, 30);

    // window coordinate
    // painter->setWindow(QRect(-50, -50, 100, 100));



    painter->setPen(pen);
    painter->setBrush(brush);
    if (antialiased)
        painter->setRenderHint(QPainter::Antialiasing, true);


    // draw polygon
//   static const QPointF points[6] = {
//        QPointF(-0.5, 0.866+c),
//        QPointF(0.5, 0.866+c),
//        QPointF(1, 0+c),
//        QPointF(0.5, -0.866+c),
//        QPointF(-0.5, -0.866+c),
//        QPointF(-1,0+c)
//    };
//    this->painter->drawPolygon(points, 6);

    //drawHexagon(1);

//------- a try to use QPolygonF----------
//    QVector<QPointF> vector(6);
//    vector.append(QPointF(-0.5, 0.866));
//    vector.append(QPointF(0.5, 0.866));
//    vector.append(QPointF(1, 0));
//    vector.append(QPointF(0.5, -0.866));
//    vector.append(QPointF(-0.5, -0.866));
//    vector.append(QPointF(-1,0));
//    QPolygonF hexagon(vector);
//    painter->drawPolygon(hexagon);
//-----------------------------------------

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setPen(palette().dark().color());
    painter->setBrush(Qt::NoBrush);
    // frame rect
    painter->drawRect(QRect(0, 0, width() - 1, height() - 1));

    painter->restore();

}
