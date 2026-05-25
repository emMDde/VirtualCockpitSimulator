#include "mainpage.h"

MainPage::MainPage(int windowHeight, int windowWidth, QWidget *parent) : QWidget(parent), _barHeight(windowHeight * 0.08)
{
    TopBar *topPanel = new TopBar(this, windowHeight * 0.08, windowWidth);
    SimulatorArea *gameArea = new SimulatorArea(this);
    _horizon = new VirtualHorizon(windowHeight * 0.5, windowWidth/5, this);
    _horizon->move(windowWidth*0.7, windowHeight*0.25);
    // QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    // opacityEffect->setOpacity(0.85);
    // _horizon->setGraphicsEffect(opacityEffect);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(topPanel);
    mainLayout->addWidget(gameArea);

    connect(topPanel, &TopBar::pageButtonClicked, this, &MainPage::changePageRequested);
    connect(topPanel, &TopBar::themeButtonClicked, this, &MainPage::changeThemeRequested);
    connect(this, &MainPage::connectionStatus, topPanel, &TopBar::setConnectionStatus);
    connect(this, &MainPage::updateSimData, gameArea, &SimulatorArea::setData);
    connect(this, &MainPage::changeThemeRequested, gameArea, &SimulatorArea::changeTheme);
    connect(this, &MainPage::updateSimData, _horizon, &VirtualHorizon::setData);
}

void MainPage::resizeEvent(QResizeEvent *event)
{
    int w = width();
    int h = height();
    _horizon->setGeometry(w * 0.7, _barHeight + h * 0.17, w / 5, h * 0.5);

    QWidget::resizeEvent(event);
}