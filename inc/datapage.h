#ifndef DATAPAGE_H
#define DATAPAGE_H

#include <QObject>
#include <QWidget>
#include "bottombar.h"
#include "flightdataarea.h"
#include "virtualhorizon.h"

/**
 * @class DataPage
 * @brief Klasa reprezentująca dedykowany widok/stronę z prezentującą dane lotu.
 *
 * Klasa dziedziczy po QWidget i służy jako kontener dla podelementów interfejsu
 * użytkownika, takich jak panel dolny (BottomBar) oraz główny obszar prezentacji
 * danych telemetrycznych (FlightDataArea). Zarządza dystrybucją sygnałów sterujących
 * oraz pośredniczy w dynamicznej zmianie motywów graficznych swoich komponentów.
 */
class DataPage : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Sygnał zgłaszający żądanie przełączenia bieżącej strony widoku.
     */
    void changePageRequested();

    /**
     * @brief Sygnał zgłaszający żądanie globalnej zmiany motywu kolorystycznego aplikacji.
     */
    void changeThemeRequested();

    /**
     * @brief Sygnał przekazujący dalej informację o stanie połączenia z kontrolerem.
     * @param[out] status -- Wartość true oznacza poprawne połączenie; false oznacza rozłączenie.
     */
    void connectionStatus(bool status);

    /**
     * @brief Sygnał przekazujący obliczone dane symulacji do składowych komponentów.
     * @param[out] rotX -- Wartość wychylenia osi X (pochylenie).
     * @param[out] rotY -- Wartość wychylenia osi Y (przechylenie).
     */
    void updateSimData(float rotX, float rotY);

    /**
     * @brief Sygnał przekazujący rozkaz uruchomienia lub zatrzymania taktowania symulacji.
     * @param[out] status -- Wartość true uruchamia symulację; false zatrzymuje ją.
     */
    void startSimulation(bool status);

public:

    /**
     * @brief Konstruktor klasy DataPage.
     * @param[in] windowHeight -- Początkowa wysokość okna w pikselach, używana do skalowania widoków.
     * @param[in] windowWidth -- Początkowa szerokość okna w pikselach, używana do skalowania widoków.
     * @param[in, out] parent -- Wskaźnik na widget nadrzędny (domyślnie nullptr).
     */
    explicit DataPage(int windowHeight, int windowWidth, QWidget *parent = nullptr);

public slots:

    /**
     * @brief Slot odpowiedzialny za odświeżenie i zmianę motywu graficznego wewnętznych paneli.
     */
    void changeTheme();

private:
    BottomBar *_bottomPanel; /**< Zmienna zawierająca uchwyt do dolnego panelu opcji. */
    FlightDataArea *_dataArea; /**< Wskażnik na widget odpowiedzialny za wyświetlanie parametrów lotu. */

};

#endif // DATAPAGE_H
