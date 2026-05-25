#ifndef SIMULATORAREA_H
#define SIMULATORAREA_H

#include <QWidget>
#include <QFrame>
#include <QPainter>
#include <QPixmap>

class SimulatorArea : public QFrame
{
    Q_OBJECT
public:
    explicit SimulatorArea(QWidget *parent = nullptr);

public slots:
    void setData(float rotX, float rotY);

    void changeTheme();

signals:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    float _rotX;
    float _rotY;
    float _scrollOffset;
    float _animSpeed;
    float _propellerRot;
    QPixmap _planeImage;
    QPixmap _treeImage;
    QPixmap _propellerImage;

    void drawBackground(QPainter &painter);
    void drawCloud(QPainter &painter, double x, double y, double cW, double cH);
};

#endif // SIMULATORAREA_H
