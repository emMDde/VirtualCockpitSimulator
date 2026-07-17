#include "simulatorarea.h"

#define FPS 50

SimulatorArea::SimulatorArea(QWidget *parent) : QFrame(parent), _frameTimer(new QTimer(this)), _simTime(0.0f), _rotX(0), _rotY(0)
{
    this->setStyleSheet("background-color: #87CEEB;");
    _treeImage.load(":/grafika/niebo.png");
    _planeImage.load(":/grafika/plane.png");
    _propellerImage.load(":/grafika/smigla.png");
    _animSpeed=5;

    connect(_frameTimer, &QTimer::timeout, this, SimulatorArea::frameTimerTick);
}

void SimulatorArea::simulationStatus(bool status)
{
    if(status) _frameTimer->start(1000/FPS);
    else _frameTimer->stop();
}

void SimulatorArea::frameTimerTick()
{
    _simTime = fmodf(_simTime + 1.5f, 46000.0f);
    _propellerRot = fmod(_propellerRot + 40.0, 360.0);
    this->update();
}

void SimulatorArea::setData(float rotX, float rotY)
{
    _rotX = rotX;
    _rotY = rotY;
}

void SimulatorArea::changeTheme()
{
    if(ToolBar::getTheme())
    {
        _skyColorTop     = QColor(0x98D1F2);
        _skyColorBottom  = QColor(0xB8D7EE);
        _groundColor     = QColor(0x61b087);
        _pathColor       = QColor(0x93999B);
        _pathWalkColor   = QColor(0xCBCFCD);
        _cloudColor      = Qt::white;
        _lineColor       = Qt::white;
    }
    else
    {
        _skyColorTop     = QColor(0xBBE4FF);
        _skyColorBottom  = QColor(0xE6F5FF);
        _groundColor     = QColor(0x61b087);//QColor("#86D9A6");
        _pathColor       = QColor(0xAAB3B8);
        _pathWalkColor   = QColor(0xE8ECEF);
        _cloudColor      = Qt::white;
        _lineColor       = QColor(0xFFEA00);
    }

    this->update();
}

void SimulatorArea::drawBackground(QPainter &painter)
{
    int w = width();
    int h = height();

    double horizonY = h * 0.585;
    QPointF vp(w / 2.0, horizonY);

    QLinearGradient skyGradient(0, 0, 0, h);
    skyGradient.setColorAt(0.0, _skyColorTop);
    skyGradient.setColorAt(0.5, _skyColorBottom);
    painter.setBrush(skyGradient);
    painter.setPen(Qt::NoPen);

    painter.drawRect(0, 0, w, h);
    painter.fillRect(0, horizonY, w, h, _groundColor); //5B9E77
    painter.drawPixmap(0, 0, w, h, _treeImage);

    double topWidth = w * 0.025;
    double bottomWidth = w * 0.46;

    QPolygonF path;
    path << QPointF(vp.x() - topWidth, horizonY) << QPointF(vp.x() + topWidth, horizonY) << QPointF(vp.x() + bottomWidth, h) << QPointF(vp.x() - bottomWidth, h);
    painter.setBrush(_pathColor);
    painter.drawPolygon(path);

    QPolygonF leftPathWalk;
    leftPathWalk << QPointF(vp.x() - topWidth, horizonY) << QPointF(vp.x() -topWidth + w*0.01, horizonY) << QPointF(vp.x() - bottomWidth + w*0.08, h) << QPointF(vp.x() - bottomWidth , h);
    painter.setBrush(_pathWalkColor);
    painter.drawPolygon(leftPathWalk);

    QPolygonF rightPathWalk;
    rightPathWalk << QPointF(vp.x() + topWidth - w*0.01, horizonY) << QPointF(vp.x() +topWidth , horizonY) << QPointF(vp.x() + bottomWidth , h) << QPointF(vp.x() + bottomWidth- w*0.08 , h);
    painter.drawPolygon(rightPathWalk);
}

void SimulatorArea::drawCloud(QPainter &painter, double x, double y, double cW, double cH)
{
    painter.setBrush(_cloudColor);
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

    // Animacja chmur
    for (int i = 0; i < 8; ++i)
    {
        double animationPosition = _simTime * 4 + (i * 200);

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

    // Animacja pasów
    double roadHeight = h - horizonY;

    double zFar = 100.0;
    double zNear = 8.0;
    double fFar = 1.0 / zFar;
    double fNear = 1.0 / zNear;
    double fRange = fNear - fFar;
    double zLineW = 6.0;

    for (int i=0; i<7; ++i)
    {
        double zTop = zFar - fmod(_simTime * 1.0f + (i * 13.5), zFar - zNear);
        double zBottom = zTop - zLineW;

        if (zTop <= zNear) continue;

        double coeffTop    = ((1.0 / zTop) - fFar) / fRange;
        double coeffBottom = ((1.0 / zBottom) - fFar) / fRange;

        double yTop    = horizonY + roadHeight * coeffTop;
        double yBottom = horizonY + roadHeight * coeffBottom;

        double maxWidth = w * 0.018;
        double wTop    = maxWidth * ((1.0 / zTop) / fNear);
        double wBottom = maxWidth * ((1.0 / zBottom) / fNear);

        if (yBottom > yTop && yTop >= horizonY)
        {
            QPolygonF stripe;
            stripe << QPointF(vp.x() - wTop, yTop) << QPointF(vp.x() + wTop, yTop) << QPointF(vp.x() + wBottom, yBottom) << QPointF(vp.x() - wBottom, yBottom);

            painter.setBrush(_lineColor);
            painter.setPen(Qt::NoPen);
            painter.drawPolygon(stripe);
        }
    }

    painter.save();
    double pitchOffset = _rotX * (h * 0.015);
    double planeY = qBound(h * 0.1, (h * 0.5) - pitchOffset, h * 0.9);

    painter.translate(w / 2.0, planeY);
    painter.rotate(_rotY);

    double targetWidth = qMin(w, h) * 0.8;
    double aspect = static_cast<double>(_planeImage.height()) / _planeImage.width();
    double targetHeight = targetWidth * aspect;
    double hw = targetWidth / 2.0;
    double hh = targetHeight / 2.0;

    // Tworzenie pochylenia - perspektywy dla samolotu
    QPolygonF sourcePoly;
    sourcePoly << QPointF(0, 0) << QPointF(_planeImage.width(), 0) << QPointF(_planeImage.width(), _planeImage.height()) << QPointF(0, _planeImage.height());

    double maxVisualPitch = 25.0;
    double pitchFactor = qBound(-1.0, _rotX / maxVisualPitch, 1.0);

    double topW  = hw * (1.0 - qAbs(pitchFactor) * 0.1);
    double bottomW = hw * (1.0 + qAbs(pitchFactor) * 0.1);
    double perspectiveH = hh * (1.0 + pitchFactor * 0.25);

    QPolygonF targetPoly;
    if (pitchFactor >= 0)
    {
        targetPoly << QPointF(-topW, -perspectiveH) << QPointF(topW, -perspectiveH)
                   << QPointF(bottomW, perspectiveH) << QPointF(-bottomW, perspectiveH);
    }
    else
    {
        targetPoly << QPointF(-bottomW, -perspectiveH) << QPointF(bottomW, -perspectiveH)
                   << QPointF(topW, perspectiveH) << QPointF(-topW, perspectiveH);
    }

    QTransform perspectiveTransform;
    if (QTransform::quadToQuad(sourcePoly, targetPoly, perspectiveTransform)) painter.setTransform(perspectiveTransform, true);

    // Animacja śmigła samolotu
    painter.save();
    double pivotX = _propellerImage.width() / 2.03;
    double pivotY = _propellerImage.height() * 0.426;
    painter.translate(pivotX, pivotY);
    painter.rotate(_propellerRot);
    painter.translate(-pivotX, -pivotY);

    painter.drawPixmap(0, 0, _propellerImage);
    painter.rotate(-_propellerRot);
    painter.restore();

    // Rysowanie samolotu
    painter.drawPixmap(0, 0, _planeImage);
    painter.restore();
}

// Zwykłe pasy
// QPen dashPen(_lineColor, qMax(2, w / 150), Qt::DashLine);
// dashPen.setDashPattern({5.0, 5.0});
// dashPen.setDashOffset(_animSpeed * (-10));
// painter.setPen(dashPen);
// painter.drawLine(QPointF(vp.x(), horizonY), QPointF(vp.x(), h));

// Las w perspektywie
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