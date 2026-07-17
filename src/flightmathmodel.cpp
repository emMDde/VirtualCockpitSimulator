#include "flightmathmodel.h"
#include <QDebug>

FlightMathModel::FlightMathModel() : _angleX(0), _angleY(0), _speedX(0.005f), _speedY(0.02f)
{
    _isActive = false;
    _isAutoLevelingX = false;
    _isAutoLevelingY = false;

    _timerX = new QTimer(this);
    _timerX->setSingleShot(true);
    _timerY = new QTimer(this);
    _timerY->setSingleShot(true);

    connect(_timerX, &QTimer::timeout, this, [this]() { _isAutoLevelingX = true; });
    connect(_timerY, &QTimer::timeout, this, [this]() { _isAutoLevelingY = true; });
}

void FlightMathModel::dataReceived(float rotX, float rotY)
{
    if(!_isActive) return;
    //qDebug() << rotX << rotY;
    float deadzone = 1.0f;

    if (std::abs(rotY) < deadzone)
    {
        if (!_timerY->isActive() && !_isAutoLevelingY) _timerY->start(1500);
    }
    else
    {
        _timerY->stop();
        _isAutoLevelingY = false;
    }

    if (_isAutoLevelingY)
    {
        _angleY = _angleY * 0.995f;

        if (std::abs(_angleY) < 0.01f)
        {
            _angleY = 0.0f;
            _isAutoLevelingY = false;
        }
    }
    else
    {
        _angleY += _speedY * rotY;
        if(_angleY>180.f) _angleY-=360.f;
        if(_angleY<-180.f) _angleY+=360.f;
    }



    if (std::abs(rotX) < deadzone)
    {
        if (!_timerX->isActive() && !_isAutoLevelingX) _timerX->start(1500);
    }
    else
    {
        _timerX->stop();
        _isAutoLevelingX = false;
    }

    if (_isAutoLevelingX)
    {
        _angleX = _angleX * 0.997f;

        if (std::abs(_angleX) < 0.01f)
        {
            _angleX = 0.0f;
            _isAutoLevelingX = false;
        }
    }
    else
    {
        _angleX += _speedX * rotX;
        // Rozpoznawanie tutaj pikowania w doł lub zbyt ostro w góre i jakaś animacje w tym stanie
    }

    emit updateSimData(_angleX, _angleY);
}

void FlightMathModel::simulationStatus(bool status)
{
    _isActive=status;
}
