#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

#include <QObject>
#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

class ControllerManager : public QObject
{
    Q_OBJECT
public:
    explicit ControllerManager(QObject *parent = nullptr);
    void start();

signals:
    void dataReceived(float rotX, float rotY);
    void connectionStatus(bool status);

private slots:
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void tryReconnect();
    void noRecentData();
    void generateTestData();

private:
    QSerialPort *_serial;
    QTimer* _reconnectTimer;
    QTimer* _watchDataTimer;

    float _testStep;

    QByteArray _buffer;
    QString _portName;
    const int PACKET_SIZE = 18;
    const int HEADER_SIZE = 8;

    bool searchForController();
    bool checkCRC(const QByteArray &data, uint16_t readCRC);

};

#endif // CONTROLLERMANAGER_H
