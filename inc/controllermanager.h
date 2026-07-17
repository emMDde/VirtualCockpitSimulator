#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

#include <QObject>
#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

/**
 * @class ControllerManager
 * @brief Zarządza komunikacją z zewnętrznym kontrolerem poprzez port szeregowy (UART).
 *
 * Klasa odpowiada za automatyczne wyszukiwanie odpowiedniego kontrolera w systemie,
 * bezpieczny odczyt i parsowanie strumienia danych binarnych, weryfikację sum kontrolnych CRC
 * oraz obsługę awarii (watchdog braku danych oraz automatyczne wznawianie połączenia).
 * W przypadku braku fizycznego urządzenia, umożliwia pracę w trybie demonstracyjnym/testowym.
 */
class ControllerManager : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief Konstruktor klasy ControllerManager.
     * @param[in, out] parent -- Wskaźnik na obiekt nadrzędny QObject.
     */
    explicit ControllerManager(QObject *parent = nullptr);

    /**
     * @brief Inicjuje menedżer i uruchamia procedurę wyszukiwania oraz połączenia z kontrolerem.
     */
    void start();

signals:

    /**
     * @brief Sygnał emitowany po odebraniu, sparsowaniu i pomyślnej weryfikacji sumy CRC pakietu danych.
     * @param[out] rotX -- Wartość wychylenia osi X (pochylenie), przekazana przez urządzenie zewnętrzne.
     * @param[out] rotY -- Wartość wychylenia osi Y (przechylenie), przekazana przez urządzenie zewnętrzne.
     */
    void dataReceived(float rotX, float rotY);

    /**
     * @brief Sygnał informujący o zmianie statusu połączenia z fizycznym kontrolerem.
     * @param[out] status -- Wartość true oznacza poprawne połączenie; false oznacza rozłączenie/brak urządzenia.
     */
    void connectionStatus(bool status);

private slots:

    /**
     * @brief Slot obsługujący zdarzenie gotowości danych w porcie szeregowym (readyRead).
     * Pobiera bajty do wewnętrznego bufora i wywołuje procedurę parsowania.
     */
    void readData();

    /**
     * @brief Slot reagujący na błędy sprzętowe i systemowe portu szeregowego.
     * Wywoływany m.in. w momencie nagłego wyciągnięcia wtyczki USB.
     * @param[in] error -- Kod błędu przekazany przez obiekt QSerialPort.
     */
    void handleError(QSerialPort::SerialPortError error);

    /**
     * @brief Slot wywoływany cyklicznie przez timer rekon koneksji.
     * Podejmuje próby ponownego odnalezienia i otwarcia portu kontrolera.
     */
    void tryReconnect();

    /**
     * @brief Slot pełniący funkcję programowego watchdoga.
     * Reaguje w sytuacji, gdy port jest otwarty, ale urządzenie przestało wysyłać pakiety danych.
     */
    void noRecentData();

    /**
     * @brief Slot generujący sztuczne, płynnie zmieniające się dane telemetryczne.
     * Używany jako fallback w trybie demonstracyjnym, gdy żaden kontroler nie jest dostępny.
     */
    void generateTestData();

private:

    QSerialPort *_serial;         /**< Wskaźnik na obiekt portu szeregowego odpowiedzialny za niskopoziomową transmisję danych. */
    QTimer* _reconnectTimer;      /**< Timer odmierzający interwały czasowe pomiędzy kolejnymi próbami odzyskania połączenia. */
    QTimer* _watchDataTimer;      /**< Timer pełniący rolę watchdoga, wykrywający brak nowych pakietów od urządzenia. */
    QTimer* _testTimer;           /**< Timer taktujący generowanie sztucznych danych telemetrycznych w trybie demonstracyjnym. */

    float _testStep;              /**< Krok/faza używana do inkrementacji wartości funkcji falowej w generatorze danych testowych. */

    QByteArray _buffer;           /**< Dynamiczny bufor przechowujący surowe bajty odebrane z magistrali UART przed ich sparsowaniem. */
    QString _portName;            /**< Systemowa nazwa portu szeregowego (np. "COM3" lub "ttyUSB0"), z którym aplikacja próbuje się połączyć. */
    const int PACKET_SIZE = 18;   /**< Stała określająca całkowity, oczekiwany rozmiar jednego kompletnego pakietu danych w bajtach. */
    const int HEADER_SIZE = 8;    /**< Stała określająca rozmiar nagłówka pakietu, wykorzystywana przy weryfikacji struktury ramki. */

    /**
     * @brief Iteruje po wszystkich dostępnych portach szeregowych w celu identyfikacji i otwarcia portu kontrolera.
     * @return true, jeśli urządzenie zostało znalezione i poprawnie zainicjalizowane; false w przeciwnym wypadku.
     */
    bool searchForController();

    /**
     * @brief Weryfikuje integralność danych poprzez porównanie obliczonej sumy kontrolnej z wartością zapisaną w pakiecie.
     * @param[in] data -- Referencja do tablicy bajtów zawierającej sprawdzany pakiet danych.
     * @param[in] readCRC -- Wartość sumy kontrolnej CRC odczytana bezpośrednio z bufora pakietu.
     * @return true, jeśli wyznaczona suma jest zgodna z odebraną (dane poprawne); false w przypadku wykrycia przekłamań.
     */
    bool checkCRC(const QByteArray &data, uint16_t readCRC);

};

#endif // CONTROLLERMANAGER_H
