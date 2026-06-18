#include "mainpage.h"

MainPage::MainPage(int windowHeight, int windowWidth, QWidget *parent) : QWidget(parent), _barHeight(windowHeight * 0.08)
{
    _topPanel = new TopBar(this, windowHeight * 0.08, windowWidth);
    _gameArea = new SimulatorArea(this);
    _horizon = new VirtualHorizon(windowHeight * 0.5, windowWidth/5, this);
    _horizon->move(windowWidth*0.7, windowHeight*0.25);
    // QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    // opacityEffect->setOpacity(0.85);
    // _horizon->setGraphicsEffect(opacityEffect);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(_topPanel);
    mainLayout->addWidget(_gameArea);

    connect(_topPanel, &TopBar::pageButtonClicked, this, &MainPage::changePageRequested);
    connect(_topPanel, &TopBar::themeButtonClicked, this, &MainPage::changeThemeRequested);
    connect(this, &MainPage::connectionStatus, _topPanel, &TopBar::setConnectionStatus);
    connect(this, &MainPage::updateSimData, _gameArea, &SimulatorArea::setData);
    connect(this, &Mainpage::connectionStatus, _gameArea, &SimulatorArea::handleConnectionStatus);
    connect(this, &MainPage::updateSimData, _horizon, &VirtualHorizon::setData);
    connect(this, &MainPage::changeThemeRequested, this, &MainPage::changeTheme);
    this->changeTheme();
}

void MainPage::resizeEvent(QResizeEvent *event)
{
    int w = width();
    int h = height();
    _horizon->setGeometry(w * 0.7, _barHeight + h * 0.17, w / 5, h * 0.5);

    QWidget::resizeEvent(event);
}

void MainPage::changeTheme()
{
    _topPanel->changeTheme();
    _gameArea->changeTheme();
    _horizon->changeTheme();
}