#ifndef FLIGHTMATHMODEL_H
#define FLIGHTMATHMODEL_H

#include <QObject>
#include <QTimer>

/**
 * @class FlightMathModel
 * @brief Model matematyczno-fizyczny zachowania samolotu w przestrzeni.
 *
 * Klasa odpowiada za przetwarzanie surowych sygnałów z kontrolera na fizyczne
 * wychylenia i prędkości kątowe obiektu. Realizuje algorytmy bezwładności lotu,
 * integrację czasową położeń oraz funkcję autopoziomowania (auto-leveling)
 * dla obu osi obrotu (X i Y), symulując zachowanie stabilizatora lotu.
 */
class FlightMathModel : public QObject
{
    Q_OBJECT
public slots:

    /**
     * @brief Odbiera surowe dane wychylenia drążka i przelicza je na siły/prędkości działające na model.
     * @param[in] rotX -- Surowa wartość wejściowa dla osi X (przechylenie).
     * @param[in] rotY -- Surowa wartość wejściowa dla osi Y (pochylenie).
     */
    void dataReceived(float rotX, float rotY);

    /**
     * @brief Aktywuje lub wstrzymuje wewnętrzne timery i obliczenia modelu matematycznego.
     * @param[in] status -- Wartość true uruchamia przeliczanie fizyki; false zamraża stan modelu.
     */
    void simulationStatus(bool status);

signals:

    /**
     * @brief Emituje przeliczone, wygładzone i ograniczone fizycznie kąty położenia samolotu.
     * @param[out] rotX -- Obliczony, ostateczny kąt pochylenia (Roll) w stopniach.
     * @param[out] rotY -- Obliczony, ostateczny kąt przechylenia (Pitch) w stopniach.
     */
    void updateSimData(float rotX, float rotY);

public:

    /**
     * @brief Konstruktor klasy FlightMathModel.
     * Inicjalizuje zmienne stanowe oraz konfiguruje timery taktujące fizykę.
     */
    FlightMathModel();

private:

    QTimer* _timerY;              /**< Timer taktujący kroki całkowania i obliczenia fizyczne dla osi Y (Pitch). */
    QTimer* _timerX;              /**< Timer taktujący kroki całkowania i obliczenia fizyczne dla osi X (Roll). */

    bool _isActive;               /**< Flaga określająca, czy model matematyczny i obliczenia fizyczne są aktywne. */
    bool _isAutoLevelingX;        /**< Flaga aktywująca powrót do pozycji poziomej (0 stopni) dla osi X po wycentrowaniu drążka. */
    bool _isAutoLevelingY;        /**< Flaga aktywująca powrót do pozycji poziomej (0 stopni) dla osi Y po wycentrowaniu drążka. */
    double _angleX;               /**< Aktualny, zintegrowany kąt obrotu wokół osi podłużnej (Roll) wyrażony w stopniach. */
    double _angleY;               /**< Aktualny, zintegrowany kąt obrotu wokół osi poprzecznej (Pitch) wyrażony w stopniach. */
    float _speedX;                /**< Chwilowa prędkość kątowa obrotu wokół osi X. */
    float _speedY;                /**< Chwilowa prędkość kątowa obrotu wokół osi Y. */
};

#endif // FLIGHTMATHMODEL_H
