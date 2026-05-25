#include "virtualhorizon.h"

VirtualHorizon::VirtualHorizon(int height, int width, QWidget *parent) : QWidget(parent)
{
    this->setMinimumSize(width/5,height/5);
    _baseHeight=height;
    _baseWidth=width;
}

void VirtualHorizon::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();
    double R = qMin(w, h) / 2.0;
    double size = R * 3.0;

    painter.translate(w / 2.0, h / 2.0);
    painter.rotate(_rotX);

    double pixelsPerDegree = R / 45.0;
    double yOffset = _rotY * pixelsPerDegree;

    painter.translate(0, yOffset);


    // QLinearGradient skyGradient(0, -size, 0, 0);
    // skyGradient.setColorAt(0.0, QColor(0x1A5B8C));
    // skyGradient.setColorAt(1.0, QColor(0x66C2FF));
    // painter.fillRect(QRectF(-size, -size, size * 2, size), skyGradient);
    painter.fillRect(QRectF(-size, -size, size * 2, size), QColor(0x3EB4E4));
    painter.fillRect(QRectF(-size, 0, size * 2, size), QColor(0x704B00));

    painter.setPen(QPen(Qt::white, R * 0.02));
    painter.drawLine(QPointF(-size, 0), QPointF(size, 0));

    for(int i = -30; i <= 30; i += 10)
    {
        if(i == 0) continue;

        double y = -i * pixelsPerDegree;
        double lineWidth = (i % 20 == 0) ? (R * 0.4) : (R * 0.2);
        painter.drawLine(QPointF(-lineWidth, y), QPointF(lineWidth, y));
    }

    painter.resetTransform();
    painter.translate(w / 2.0, h / 2.0);
    painter.setPen(QPen(Qt::white, R * 0.03));
    double innerGap = R * 0.2;
    double wingLength = R * 0.6;
    double noseSize = R * 0.1;

    painter.drawLine(QPointF(-wingLength, 0), QPointF(-innerGap, 0));
    painter.drawLine(QPointF(innerGap, 0), QPointF(wingLength, 0));
    painter.drawPolyline(QPolygonF() << QPointF(-noseSize, noseSize) << QPointF(0, 0) << QPointF(noseSize, noseSize));
}

void VirtualHorizon::setData(float rotX, float rotY)
{
    _rotX=rotX;
    _rotY=rotY;
    this->update();
}

QSize VirtualHorizon::sizeHint() const
{
    return QSize(_baseWidth, _baseHeight);
}

