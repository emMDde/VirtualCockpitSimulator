#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _controller(new ControllerManager(this))
{
    setWindowTitle(tr("Symulator Wirtualnego Kokpitu"));

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    _windowWidth = screenWidth * 0.8;
    _windowHeight = screenHeight * 0.8;
    this->resize(_windowWidth, _windowHeight);

    int x = (screenWidth - _windowWidth) / 2;
    int y = (screenHeight - _windowHeight) / 2;
    this->move(x, y);

    createLayout();
    _controller->start();
}

void MainWindow::createLayout()
{
    QStackedWidget *stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    MainPage *mainPage = new MainPage(_windowHeight, _windowWidth, this);
    DataPage *dataPage = new DataPage(_windowHeight, _windowWidth, this);

    stackedWidget->addWidget(mainPage);
    stackedWidget->addWidget(dataPage);
    stackedWidget->setCurrentIndex(0);

    connect(_controller, &ControllerManager::connectionStatus, mainPage, &MainPage::connectionStatus);
    connect(_controller, &ControllerManager::dataReceived, mainPage, &MainPage::updateSimData);
    connect(_controller, &ControllerManager::connectionStatus, dataPage, &DataPage::connectionStatus);
    connect(_controller, &ControllerManager::dataReceived, dataPage, &DataPage::updateSimData);

    connect(mainPage, &MainPage::changePageRequested, this, [=]() {
        stackedWidget->setCurrentIndex(1);
    });

    connect(dataPage, &DataPage::changePageRequested, this, [=]() {
        stackedWidget->setCurrentIndex(0);
    });
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) reTranslate();
    QMainWindow::changeEvent(event);
}

void MainWindow::reTranslate()
{
    setWindowTitle(tr("Symulator Wirtualnego Kokpitu"));
}
