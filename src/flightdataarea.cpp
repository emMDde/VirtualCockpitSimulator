#include "flightdataarea.h"

FlightDataArea::FlightDataArea(int height, int width, QWidget *parent) : QFrame(parent), _maxDataPoints(400), _isPaused(false)
{
    _layoutContainer = new QFrame(this);
    //_layoutContainer->setStyleSheet("background: transparent; border: none;");
    _horizon = new VirtualHorizon(height * 0.83, width*0.25, this);
    // QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    // opacityEffect->setOpacity(0.85);
    // _horizon->setGraphicsEffect(opacityEffect);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(width*0.08, height*0.035, width*0.05, height*0.035);
    mainLayout->addWidget(_horizon, 0, Qt::AlignVCenter);
    mainLayout->setSpacing(width*0.07);
    mainLayout->addWidget(_layoutContainer);

    _titleLabel = new QLabel(tr("DANE LOTU"), _layoutContainer);
    _titleLabel->setAlignment(Qt::AlignCenter);
    _titleLabel->setStyleSheet("font-family: Arial; font-size: 18px; font-weight: bold; color: black;");

    _xGraphSpace = new QWidget(_layoutContainer);
    _xGraphSpace->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _yGraphSpace = new QWidget(_layoutContainer);
    _yGraphSpace->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _pauseBtn = new QPushButton(tr("PAUZA"), _layoutContainer);
    _clearBtn = new QPushButton(tr("WYCZYŚĆ"), _layoutContainer);
    QString btnStyle =
        "QPushButton { "
        "   background-color: #555555; "
        "   color: #FFFFFF; "
        "   border: 2px solid #3A3A3A; "
        "   border-radius: 4px; "
        "   padding: 8px; "
        "   font-weight: bold; "
        "   font-size: 14px; "
        "}"
        "QPushButton:hover { "
        "   background-color: #6A6A6A; "
        "   border: 2px solid #3A9BDC; "
        "}"
        "QPushButton:pressed { "
        "   background-color: #404040; "
        "   border: 3px solid #D94040; "
        "   color: #E0E0E0; "
        "   padding: 10px 8px 6px 8px; "
        "}";
    _pauseBtn->setStyleSheet(btnStyle);
    _clearBtn->setStyleSheet(btnStyle);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(_pauseBtn);
    btnLayout->addWidget(_clearBtn);

    QVBoxLayout *panelLayout = new QVBoxLayout(_layoutContainer);
    panelLayout->setContentsMargins(width*0.01, height*0.02, width*0.01, height*0.02);
    panelLayout->setSpacing(height*0.01);
    panelLayout->addWidget(_titleLabel);
    panelLayout->addWidget(_xGraphSpace);
    panelLayout->addWidget(_yGraphSpace);
    panelLayout->addLayout(btnLayout);

    connect(_pauseBtn, &QPushButton::clicked, this, &FlightDataArea::togglePause);
    connect(_clearBtn, &QPushButton::clicked, this, &FlightDataArea::clearData);
    connect(this, &FlightDataArea::updateSimData, _horizon, &VirtualHorizon::setData);
    connect(this, &FlightDataArea::updateSimData, this, &FlightDataArea::setData);
}

void FlightDataArea::resizeEvent(QResizeEvent *event)
{
    int w = width();
    int h = height();
    _horizon->setFixedSize(w * 0.25, h * 0.83);

    QFrame::resizeEvent(event);
}

void FlightDataArea::changeTheme()
{

}

void FlightDataArea::setData(float rotX, float rotY)
{
    if (_isPaused) return;

    _rollHistory.append(rotX);
    _pitchHistory.append(rotY);

    if (_rollHistory.size()>_maxDataPoints)
    {
        _rollHistory.removeFirst();
        _pitchHistory.removeFirst();
    }
    this->update();
}

void FlightDataArea::togglePause()
{
    _isPaused = !_isPaused;
    _pauseBtn->setText(_isPaused ? tr("WZNÓW") : tr("PAUZA"));
}

void FlightDataArea::clearData()
{
    _rollHistory.clear();
    _pitchHistory.clear();
    update();
}

void FlightDataArea::reTranslate()
{
    _pauseBtn->setText(_isPaused ? tr("WZNÓW") : tr("PAUZA"));
    _clearBtn->setText(tr("WYCZYŚĆ"));
    _titleLabel->setText(tr("DANE LOTU"));
}

void FlightDataArea::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) reTranslate();
    QFrame::changeEvent(event);
}

void FlightDataArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();
    painter.fillRect(0, 0, w, h, QColor(0x99CDEB));

    auto drawCloud = [&](int x, int y, int cW, int cH) {
        painter.setBrush(Qt::white);
        painter.setPen(Qt::NoPen);
        double cornerRadius = cH * 0.4;
        painter.drawRoundedRect(x, y + cH * 0.4, cW, cH * 0.6, cornerRadius, cornerRadius);

        painter.drawEllipse(x + cW * 0.08, y + cH * 0.2, cW * 0.4, cH * 0.7);
        painter.drawEllipse(x + cW * 0.28, y, cW * 0.48, cH * 0.9);
        painter.drawEllipse(x + cW * 0.60, y + cH * 0.25, cW * 0.35, cH * 0.6);
    };

    drawCloud(w * 0.03, h * 0.15, w * 0.12, h * 0.10);
    drawCloud(w * 0.25, h * 0.05, w * 0.15, h * 0.12);
    drawCloud(w * 0.80, h * 0.04, w * 0.18, h * 0.14);

    int bottomY = h * 0.83;
    double peakY = h * 0.68;

    QPainterPath groundShape;
    groundShape.moveTo(0, bottomY);
    groundShape.quadTo(w / 2.0, peakY, w, bottomY);
    groundShape.lineTo(w, h);
    groundShape.lineTo(0, h);
    groundShape.closeSubpath();

    painter.setBrush(QColor(0x555555));
    painter.setPen(Qt::NoPen);
    painter.drawPath(groundShape);

    QPainterPath ridgeLine;
    ridgeLine.moveTo(0, bottomY);
    ridgeLine.quadTo(w / 2.0, peakY, w, bottomY);

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(0x777777), h*0.03, Qt::SolidLine, Qt::RoundCap));
    painter.drawPath(ridgeLine);

    QRect horizon = _horizon->geometry();
    painter.setPen(QPen(QColor(0x404040), h*0.016));
    painter.drawRect(horizon);

    QRect panelRect = _layoutContainer->geometry();
    painter.fillRect(panelRect, QColor(0xEAEAEA));
    painter.setPen(QPen(QColor(0x404040), h*0.008));
    painter.drawRect(panelRect);

    QRect rollRect = _xGraphSpace->geometry().translated(panelRect.topLeft());
    QRect pitchRect = _yGraphSpace->geometry().translated(panelRect.topLeft());

    painter.fillRect(rollRect, Qt::black);
    painter.fillRect(pitchRect, Qt::black);

    int padL = 40;
    int padB = 20;
    int padT = 10;

    auto drawGraph = [&](const QRect& rect, const QList<float>& data, QColor color, float maxVal, const QString& topTxt, const QString& botTxt) {

        int plotX = rect.x() + padL;
        int plotY = rect.y() + padT;
        int plotW = rect.width() - padL - 10;
        int plotH = rect.height() - padT - padB;
        int zeroY = plotY + (plotH / 2);

        painter.setPen(QPen(Qt::white, 2));
        painter.drawLine(plotX, plotY, plotX, plotY + plotH); // Oś Y
        painter.drawLine(plotX, plotY + plotH, plotX + plotW, plotY + plotH); // Oś X

        painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter.drawLine(plotX, zeroY, plotX + plotW, zeroY);

        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 8, QFont::Bold));
        painter.drawText(rect.x(), plotY, padL - 5, 20, Qt::AlignRight | Qt::AlignTop, topTxt);
        painter.drawText(rect.x(), zeroY - 10, padL - 5, 20, Qt::AlignRight | Qt::AlignVCenter, "0");
        painter.drawText(rect.x(), plotY + plotH - 20, padL - 5, 20, Qt::AlignRight | Qt::AlignBottom, botTxt);

        if (data.size() > 1) {
            painter.setPen(QPen(color, 3));
            float stepX = (float)plotW / (_maxDataPoints - 1);

            for (int i = 0; i < data.size() - 1; ++i) {
                float y1 = zeroY - (data[i] * (plotH / (maxVal * 2.0)));
                float y2 = zeroY - (data[i+1] * (plotH / (maxVal * 2.0)));

                float x1 = plotX + (i * stepX);
                float x2 = plotX + ((i + 1) * stepX);

                painter.drawLine(x1, y1, x2, y2);
            }
        }
    };

    drawGraph(rollRect, _rollHistory, QColor(0x3A9BDC), 180.0f, "+180", "-180");
    drawGraph(pitchRect, _pitchHistory, QColor(0xD94040), 90.0f, "+90", "-90");
}