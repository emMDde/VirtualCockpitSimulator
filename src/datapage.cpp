#include "datapage.h"

DataPage::DataPage(int windowHeight, int windowWidth, QWidget *parent) : QWidget(parent)
{
    _bottomPanel = new BottomBar(this, windowHeight * 0.08, windowWidth);
    _dataArea = new FlightDataArea(windowHeight*0.92, windowWidth, this);
    //VirtualHorizon *horizon = new VirtualHorizon(windowHeight * 0.7, windowWidth/4, this);
    //horizon->move(windowWidth*0.1, windowHeight*0.08);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(_dataArea);
    mainLayout->addWidget(_bottomPanel);

    connect(_bottomPanel, &BottomBar::pageButtonClicked, this, &DataPage::changePageRequested);
    connect(_bottomPanel, &BottomBar::themeButtonClicked, this, &DataPage::changeThemeRequested);
    connect(this, &DataPage::connectionStatus, _bottomPanel, &BottomBar::setConnectionStatus);
    connect(this, &DataPage::updateSimData, _dataArea, &FlightDataArea::updateSimData);
    connect(this, &DataPage::changeThemeRequested, this, &DataPage::changeTheme);
    //connect(this, &DataPage::updateSimData, horizon, &VirtualHorizon::setData);
    //connect(this, &DataPage::startSimulation, _dataArea, &FlightDataArea::startSimulation);
    this->changeTheme();
}

void DataPage::changeTheme()
{
    _bottomPanel->changeTheme();
    _dataArea->changeTheme();
}
