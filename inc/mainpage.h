#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QObject>
#include <QWidget>
#include "topbar.h"
#include "simulatorarea.h"
#include "virtualhorizon.h"
#include "entryoverlay.h"

/**
 * @class MainPage
 * @brief Główny widok aplikacji integrujący trójwymiarową symulację lotu z interfejsem użytkownika.
 *
 * Klasa dziedziczy po QWidget i stanowi kontener (stronę) organizujący kluczowe elementy symulatora:
 * górny panel nawigacyjny (TopBar), silnik renderujący obszar symulacji (SimulatorArea),
 * nałożony na niego wirtualny horyzont (VirtualHorizon) oraz ekran pauzy/oczekiwania (EntryOverlay).
 * Pełni rolę dystrybutora sygnałów pomiędzy poszczególnymi komponentami tej strony.
 */
class MainPage : public QWidget
{
    Q_OBJECT
signals:

    /**
     * @brief Sygnał zgłaszający żądanie przełączenia bieżącej strony (np. przejście do strony z danymi).
     */
    void changePageRequested();

    /**
     * @brief Sygnał zgłaszający żądanie globalnej zmiany motywu kolorystycznego wewnątrz strony.
     */
    void changeThemeRequested();

    /**
     * @brief Sygnał przekazujący informację o aktualnym stanie połączenia z kontrolerem.
     * @param[out] status -- Wartość true oznacza poprawne połączenie; false oznacza jego brak.
     */
    void connectionStatus(bool status);

    /**
     * @brief Sygnał przekazujący zaktualizowane dane telemetryczne do poszczególnych widgetów.
     * @param[out] rotX -- Wartość wychylenia osi X (pochylenie) w stopniach.
     * @param[out] rotY -- Wartość wychylenia osi Y (przechylenie) w stopniach.
     */
    void updateSimData(float rotX, float rotY);

    /**
     * @brief Sygnał przekazujący rozkaz uruchomienia, wznowienia lub zatrzymania symulacji.
     * @param[out] status -- Wartość true aktywuje fizykę/renderowanie; false zatrzymuje symulację.
     */
    void simulationStatus(bool status);

protected:

    /**
     * @brief Obsługuje zdarzenie zmiany rozmiaru głównego okna aplikacji.
     * Dynamicznie przelicza i aktualizuje rozmiary oraz pozycje nakładek (VirtualHorizon, EntryOverlay).
     * @param[in] event -- Wskaźnik na obiekt zdarzenia QResizeEvent.
     */
    void resizeEvent(QResizeEvent *event) override;

public:

    /**
     * @brief Konstruktor klasy MainPage.
     * Inicjalizuje wszystkie widgety potomne, ustala ich układ (Layout) i łączy odpowiednie sygnały.
     * @param[in] windowHeight -- Początkowa wysokość okna w pikselach, używana do skalowania elementów.
     * @param[in] windowWidth -- Początkowa szerokość okna w pikselach, używana do pozycjonowania nakładek.
     * @param[in, out] parent -- Wskaźnik na widget nadrzędny (domyślnie nullptr).
     */
    explicit MainPage(int windowHeight, int windowWidth, QWidget *parent = nullptr);

public slots:

    /**
     * @brief Slot odpowiedzialny za rozesłanie żądania zmiany motywu graficznego do wszystkich widgetów potomnych.
     */
    void changeTheme();

private:

    TopBar *_topPanel;            /**< Wskaźnik na górny panel nawigacyjny (zawierający przyciski menu i statusu). */
    SimulatorArea *_gameArea;     /**< Wskaźnik na główny obszar renderowania trójwymiarowego modelu samolotu i otoczenia. */
    EntryOverlay* _entryOverlay;  /**< Wskaźnik na nakładkę interfejsu (ekran oczekiwania, pauzy lub awarii połączenia). */

    VirtualHorizon *_horizon;     /**< Wskaźnik na widżet wirtualnego horyzontu rysowany jako wskaźnik lotniczy (HUD). */
    float _barHeight;             /**< Obliczona wysokość górnego panelu w pikselach, używana do wyznaczania marginesów układu. */
};

#endif // MAINPAGE_H
