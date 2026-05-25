#ifndef DATAPAGE_H
#define DATAPAGE_H

#include <QObject>
#include <QWidget>
#include "bottombar.h"
#include "flightdataarea.h"
#include "virtualhorizon.h"

class DataPage : public QWidget
{
    Q_OBJECT
signals:
    void changePageRequested();
    void changeThemeRequested();
    void connectionStatus(bool status);
    void updateSimData(float rotX, float rotY);

public:
    explicit DataPage(int windowHeight, int windowWidth, QWidget *parent = nullptr);

signals:
};

#endif // DATAPAGE_H
