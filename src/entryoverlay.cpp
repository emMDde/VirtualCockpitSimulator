#include "entryoverlay.h"

EntryOverlay::EntryOverlay(QWidget *parent) : QFrame(parent), _isConnected(false), _dotCount(1)
{
    this->setStyleSheet("EntryOverlay { background-color: rgba(23, 24, 31, 220); }");

    _statusLabel = new QLabel(this);
    _statusLabel->setStyleSheet("color: white; font-size: 34px; font-weight: bold; background: transparent;");
    _statusLabel->setAlignment(Qt::AlignCenter);

    _startBtn = new AnimatedButton(50, 200, orange, tr("START"), this);
    _startBtn->setEnabled(false);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(_statusLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(25);
    mainLayout->addWidget(_startBtn, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    _dotTimer = new QTimer(this);
    connect(_dotTimer, &QTimer::timeout, this, [this]() {_dotCount = (_dotCount % 3) + 1;
        reTranslate();
    });

    connect(_startBtn, &AnimatedButton::clicked, this, [this]() { emit simulationStatus(true); });

    reTranslate();
    _dotTimer->start(500);

}

void EntryOverlay::handleConnection(bool connected)
{
    _startBtn->setEnabled(connected);
    _isConnected=connected;

    if(_isConnected)
    {
        _dotTimer->stop();
        _statusLabel->setStyleSheet("color: #4CAF50; font-size: 34px; font-weight: bold; background: transparent;");
    }
    else
    {
        _statusLabel->setStyleSheet("color: white; font-size: 34px; font-weight: bold; background: transparent;");
        if (!_dotTimer->isActive()) _dotTimer->start(500);
        emit simulationStatus(false);
    }

    reTranslate();
}

void EntryOverlay::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) reTranslate();

    QFrame::changeEvent(event);
}

void EntryOverlay::reTranslate()
{
    _startBtn->setText(tr("START"));

    if (_isConnected)
    {
        _statusLabel->setText(tr("Połączono"));
    }
    else
    {
        QString dots = "";
        for (int i = 0; i < _dotCount; ++i) dots += ".";
        _statusLabel->setText(tr("Łączenie z kontrolerem") + " " + dots);
    }
}