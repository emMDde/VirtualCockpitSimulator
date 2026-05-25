#include "simulatorarea.h"
#include <QPainter>
#include <cmath>
#include <QDebug>

SimulatorArea::SimulatorArea(QWidget *parent) : QFrame(parent), _scrollOffset(0.0f)
{
    this->setStyleSheet("background-color: #87CEEB;");
    _treeImage.load(":/grafika/niebo.png");
    _planeImage.load(":/grafika/plane.png");
    _propellerImage.load(":/grafika/smigla.png");
    _animSpeed=5;
}

void SimulatorArea::setData(float rotX, float rotY)
{
    _rotX = 0;//rotX;
    _rotY = rotY;

    _scrollOffset += 1.5f;
    if (_scrollOffset > 100000.0f) _scrollOffset = 0.0f;

    this->update();
}

void SimulatorArea::drawBackground(QPainter &painter)
{
    int w = width();
    int h = height();

    double horizonY = h * 0.585;
    QPointF vp(w / 2.0, horizonY);

    QLinearGradient skyGradient(0, 0, 0, h);
    skyGradient.setColorAt(0.0, QColor(0x98D1F2));
    skyGradient.setColorAt(0.5, QColor(0xB8D7EE));
    painter.setBrush(skyGradient);
    painter.setPen(Qt::NoPen);

    painter.drawRect(0, 0, w, h);
    painter.fillRect(0, horizonY, w, h, QColor(0x61b087)); //5B9E77
    painter.drawPixmap(0, 0, w, h, _treeImage);

    double topWidth = w * 0.025;
    double bottomWidth = w * 0.46;

    QPolygonF path;
    path << QPointF(vp.x() - topWidth, horizonY) << QPointF(vp.x() + topWidth, horizonY) << QPointF(vp.x() + bottomWidth, h) << QPointF(vp.x() - bottomWidth, h);
    painter.setBrush(QColor(0x93999B));
    painter.drawPolygon(path);

    QPolygonF leftPathWalk;
    leftPathWalk << QPointF(vp.x() - topWidth, horizonY) << QPointF(vp.x() -topWidth + w*0.01, horizonY) << QPointF(vp.x() - bottomWidth + w*0.08, h) << QPointF(vp.x() - bottomWidth , h);
    painter.setBrush(QColor(0xCBCFCD));
    painter.drawPolygon(leftPathWalk);

    QPolygonF rightPathWalk;
    rightPathWalk << QPointF(vp.x() + topWidth - w*0.01, horizonY) << QPointF(vp.x() +topWidth , horizonY) << QPointF(vp.x() + bottomWidth , h) << QPointF(vp.x() + bottomWidth- w*0.08 , h);
    painter.drawPolygon(rightPathWalk);
}

void SimulatorArea::drawCloud(QPainter &painter, double x, double y, double cW, double cH)
{
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    double cornerRadius = cH * 0.4;

    painter.drawRoundedRect(x, y + cH * 0.4, cW, cH * 0.6, cornerRadius, cornerRadius);
    painter.drawEllipse(x + cW * 0.08, y + cH * 0.2, cW * 0.4, cH * 0.7);
    painter.drawEllipse(x + cW * 0.28, y, cW * 0.48, cH * 0.9);
    painter.drawEllipse(x + cW * 0.60, y + cH * 0.25, cW * 0.35, cH * 0.6);
}

void SimulatorArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    drawBackground(painter);

    int w = width();
    int h = height();
    double horizonY = h * 0.585;
    QPointF vp(w / 2.0, horizonY);

    for (int i = 0; i < 8; ++i)
    {
        double animationPosition = _scrollOffset * _animSpeed + (i * 200);

        double z = 1000.0 - fmod(animationPosition, 1000.0);
        if (z < 1.0) continue;

        double scale = 100.0 / z;

        int cycle = static_cast<int>(animationPosition / 1000.0);
        const double offsets[] = { 45.0, -20.0, 75.0, -60.0, 15.0, -80.0, 30.0, -40.0 };
        const int offsetCount = 8;
        double cloudRandomOffsetX = offsets[(i + cycle) % offsetCount];
        double cloudRandomOffsetY = offsets[(i * 3 + cycle) % offsetCount] * 0.5;

        double worldX = (i % 2 == 0) ? -w * 0.3 - (i * 25) + cloudRandomOffsetX :  w * 0.2 + (i * 20) + cloudRandomOffsetX;
        double worldY = -h * 0.3 - (i * 10) + cloudRandomOffsetY;
        double cloudX = vp.x() + (worldX * scale);
        double cloudY = vp.y() + (worldY * scale);
        double cloudW = w * 0.04 * scale;
        double cloudH = h * 0.04 * scale;
        drawCloud(painter, cloudX, cloudY, cloudW, cloudH);
    }

    QPen dashPen(Qt::white, qMax(2, w / 150), Qt::DashLine);
    dashPen.setDashPattern({5.0, 5.0});
    dashPen.setDashOffset(-_scrollOffset * _animSpeed * 0.1);
    painter.setPen(dashPen);
    painter.drawLine(QPointF(vp.x(), horizonY), QPointF(vp.x(), h));

    painter.save();
    double pitchOffset = _rotY * (h * 0.015);
    double planeY = qBound(h * 0.1, (h * 0.5) - pitchOffset, h * 0.9);

    painter.translate(w / 2.0, planeY);
    painter.rotate(_rotX);

    double targetWidth = qMin(w, h) * 0.8;
    double aspect = static_cast<double>(_planeImage.height()) / _planeImage.width();
    double targetHeight = targetWidth * aspect;



    _propellerRot = fmod(_propellerRot + 30.0, 360.0);

    double pivotX = _propellerImage.width() / 2.03;
    double pivotY = _propellerImage.height() * 0.426;

    painter.save();
    painter.translate(pivotX, pivotY);
    painter.rotate(_propellerRot);

    painter.translate(-pivotX, -pivotY);

    painter.drawPixmap(0, 0, _propellerImage);
    painter.rotate(-_propellerRot);
    painter.restore();
    painter.drawPixmap(0, 0, _planeImage);
    painter.restore();
}

void SimulatorArea::changeTheme()
{

}

// ==========================================
// 5. ZAPĘTLONY LAS W PERSPEKTYWIE
// ==========================================
// Rysujemy od tyłu (z=100) do przodu (z=1), żeby bliższe drzewa zasłaniały dalsze
// for (int i = 12; i > 0; --i) {
//     double z = 100.0 - fmod(_scrollOffset * 15.0 + (i * 8.5), 100.0);
//     if (z < 1.0) continue;

//     double scale = 15.0 / z;

//     // Rozmiar grafiki lasu w danej odległości
//     int imgW = w * 0.18 * scale;
//     int imgH = h * 0.22 * scale;

//     // Chcemy, aby las stał idealnie wzdłuż krawędzi drogi.
//     // Droga rozszerza się od punktu zbiegu, więc las musi się od niego odsuwać:
//     double leftEdgeX = vp.x() - (w * 0.26 * scale) - imgW;
//     double rightEdgeX = vp.x() + (w * 0.26 * scale);

//     // Baza lasu idzie w dół ekranu wraz z przybliżaniem
//     double forestY = vp.y() + (h * 0.22 * scale) - imgH;

//     // Rysujemy lewy i prawy pas lasu
//     if (forestY < h + 50) { // optymalizacja: nie rysuj, jeśli całkowicie spadł pod ekran
//         painter.drawPixmap(QRect(leftEdgeX, forestY, imgW, imgH), _forestPixmap);
//         painter.drawPixmap(QRect(rightEdgeX, forestY, imgW, imgH), _forestPixmap);
//     }
// }