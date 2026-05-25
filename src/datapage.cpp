#include "datapage.h"

DataPage::DataPage(int windowHeight, int windowWidth, QWidget *parent) : QWidget(parent)
{
    BottomBar *bottomPanel = new BottomBar(this, windowHeight * 0.08, windowWidth);
    FlightDataArea *dataArea = new FlightDataArea(windowHeight*0.92, windowWidth, this);
    //VirtualHorizon *horizon = new VirtualHorizon(windowHeight * 0.7, windowWidth/4, this);
    //horizon->move(windowWidth*0.1, windowHeight*0.08);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(dataArea);
    mainLayout->addWidget(bottomPanel);

    connect(bottomPanel, &BottomBar::pageButtonClicked, this, &DataPage::changePageRequested);
    connect(bottomPanel, &BottomBar::themeButtonClicked, this, &DataPage::changeThemeRequested);
    connect(this, &DataPage::connectionStatus, bottomPanel, &BottomBar::setConnectionStatus);
    connect(this, &DataPage::updateSimData, dataArea, &FlightDataArea::updateSimData);
    connect(this, &DataPage::changeThemeRequested, dataArea, &FlightDataArea::changeTheme);
    //connect(this, &DataPage::updateSimData, horizon, &VirtualHorizon::setData);
}
