#include "controllermanager.h"

ControllerManager::ControllerManager(QObject *parent) : QObject(parent), _serial(new QSerialPort(this)), _reconnectTimer(new QTimer(this)), _watchDataTimer(new QTimer(this))
{
    _serial->setBaudRate(QSerialPort::Baud115200);
    _serial->setDataBits(QSerialPort::Data8);
    _serial->setParity(QSerialPort::NoParity);
    _serial->setStopBits(QSerialPort::OneStop);
    _serial->setFlowControl(QSerialPort::NoFlowControl);

    _watchDataTimer->setSingleShot(true);
    connect(_watchDataTimer, &QTimer::timeout, this, &ControllerManager::noRecentData);
    connect(_serial, &QSerialPort::errorOccurred, this, &ControllerManager::handleError);
    connect(_reconnectTimer, &QTimer::timeout, this, &ControllerManager::tryReconnect);
}

void ControllerManager::start()
{
    if(!searchForController()) _reconnectTimer->start(1000);
    //_testTimer = new QTimer(this);
    //connect(_testTimer, &QTimer::timeout, this, &ControllerManager::generateTestData);
    //_testTimer->start(20);
}

void ControllerManager::generateTestData()
{
    _testStep += 0.01f;

    float rotX = 150.0f * std::sin(_testStep);
    float rotY = 45.0f  * std::sin(_testStep * 0.5f);

    emit dataReceived(rotX, rotY);
}

bool ControllerManager::searchForController()
{
    for (const QSerialPortInfo &port : QSerialPortInfo::availablePorts())
    {
        _serial->setPort(port);

        if (_serial->open(QIODevice::ReadWrite))
        {
            _serial->write("WDS_CONTROLLER_CONNECT");
            //qDebug() << "Napisano wiadomosc na porcie:" << port.portName();

            QByteArray response;
            while(_serial->waitForReadyRead(100))
            {
                response.append(_serial->readAll());

                if (response.contains("WDS_CONTROLLER_CONNECTED") || response.contains("WDSDATA"))
                {
                    qDebug() << "WYKRYTO KONTROLER! Znaleziono urządzenie na porcie:" << port.portName();
                    _portName = port.portName();
                    connect(_serial, &QSerialPort::readyRead, this, &ControllerManager::readData);
                    emit connectionStatus(true);
                    _watchDataTimer->start(300);
                    return true;
                }
                else if(response.length()>PACKET_SIZE*2) break;
            }
            _serial->close();
        }
    }
    return false;
}

bool ControllerManager::checkCRC(const QByteArray &data, uint16_t readCRC)
{
    uint16_t crc = 0xFFFF;
    uint16_t poly = 0x8005;
    int length = PACKET_SIZE-2;
    const uint8_t *ptr = reinterpret_cast<const uint8_t*>(data.constData());

    for (int i = 0; i < length; ++i)
    {
        crc ^= (uint16_t)(ptr[i] << 8);
        for (int j = 0; j < 8; ++j) crc = (crc & 0x8000) ? (crc << 1) ^ poly : (crc << 1);

    }

    return (crc == readCRC);
}

void ControllerManager::readData()
{
    _buffer.append(_serial->readAll());

    while (_buffer.size() >= PACKET_SIZE)
    {
        int headerPos = _buffer.indexOf("WDSDATA");
        if (headerPos == -1)
        {
            _buffer.remove(0, _buffer.size()-(HEADER_SIZE-2));
            break;
        }

        if (headerPos > 0)
        {
            _buffer.remove(0, headerPos);
            if(_buffer.size()<PACKET_SIZE) break;
        }
        float rotX = 0.0f, rotY = 0.0f;
        uint16_t crc;

        std::memcpy(&rotX, _buffer.constData() + 8, sizeof(float));
        std::memcpy(&rotY, _buffer.constData() + 12, sizeof(float));
        std::memcpy(&crc, _buffer.constData() + 16, sizeof(crc));
        //qDebug() << "Otrzymano dane:" << rotX << rotY;
        if(checkCRC(_buffer, crc))
        {
            emit dataReceived(rotX, rotY);
            _watchDataTimer->start(300);
            _buffer.remove(0, PACKET_SIZE);
        }
        else _buffer.remove(0, HEADER_SIZE);
    }
}

void ControllerManager::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        disconnect(_serial, &QSerialPort::readyRead, this, &ControllerManager::readData);
        _serial->close();
        _buffer.clear();

        emit connectionStatus(false);

        if(!_reconnectTimer->isActive()) _reconnectTimer->start(1000);
    }
}

void ControllerManager::tryReconnect()
{
    _serial->close();
    _buffer.clear();
    if(searchForController())
    {
        _reconnectTimer->stop();
    }
}

void ControllerManager::noRecentData()
{
    handleError(QSerialPort::ResourceError);
}