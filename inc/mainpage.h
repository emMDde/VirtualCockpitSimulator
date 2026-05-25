#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QObject>
#include <QWidget>
#include "topbar.h"
#include "simulatorarea.h"
#include "virtualhorizon.h"

class MainPage : public QWidget
{
    Q_OBJECT
signals:
    void changePageRequested();
    void changeThemeRequested();
    void connectionStatus(bool status);
    void updateSimData(float rotX, float rotY);

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit MainPage(int windowHeight, int windowWidth, QWidget *parent = nullptr);

private:
    VirtualHorizon *_horizon;
    float _barHeight;
};

#endif // MAINPAGE_H
