#ifndef SIMULATORAREA_H
#define SIMULATORAREA_H

#include <QWidget>
#include <QFrame>
#include <QPainter>
#include <QPixmap>
#include <QPainter>
#include <cmath>
#include <QDebug>
#include <QTimer>
#include "toolbar.h"

class SimulatorArea : public QFrame
{
    Q_OBJECT
public:
    explicit SimulatorArea(QWidget *parent = nullptr);

public slots:
    void handleConnectionStatus();

    void setData(float rotX, float rotY);

    void changeTheme();

signals:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTimer* _frameTimer;
    float _rotX;
    float _rotY;
    float _scrollOffset;
    float _animSpeed;
    float _propellerRot;
    QPixmap _planeImage;
    QPixmap _treeImage;
    QPixmap _propellerImage;

    QColor _skyColorTop;
    QColor _skyColorBottom;
    QColor _groundColor;
    QColor _pathColor;
    QColor _pathWalkColor;
    QColor _cloudColor;
    QColor _lineColor;

    void drawBackground(QPainter &painter);
    void drawCloud(QPainter &painter, double x, double y, double cW, double cH);
};

#endif // SIMULATORAREA_H
