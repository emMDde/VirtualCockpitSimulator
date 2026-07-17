#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QScreen>
#include <QStackedWidget>
#include "mainpage.h"
#include "datapage.h"
#include "flightmathmodel.h"
#include "controllermanager.h"

/**
 * @class MainWindow
 * @brief Główne okno aplikacji zarządzające cyklem życia programu i globalną architekturą widoków.
 *
 * Klasa stanowi główny punkt wejściowy interfejsu graficznego (GUI). Odpowiada za
 * inicjalizację systemu zarządzania stronami (QStackedWidget), integrację niskopoziomowego
 * menedżera kontrolera (ControllerManager) z modelem fizyki lotu (FlightMathModel) oraz
 * globalną dystrybucję sygnałów sterujących i danych telemetrycznych w aplikacji.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    /**
     * @brief Konstruktor klasy MainWindow.
     * Pobiera geometrię ekranu, konfiguruje początkowy rozmiar okna, inicjalizuje
     * podsystemy sprzętowe i matematyczne oraz buduje główny układ graficzny.
     * @param[in, out] parent -- Wskaźnik na widget nadrzędny (domyślnie nullptr).
     */
    explicit MainWindow(QWidget *parent = nullptr);

private slots:


protected:

    /**
     * @brief Reaguje na globalne zmiany systemowe przesyłane do okna.
     * Nadpisuje domyślne zachowanie w celu przechwycenia zdarzenia zmiany języka (LanguageChange).
     * @param[in] event -- Wskaźnik na obiekt zdarzenia QEvent.
     */
    void changeEvent(QEvent *event) override;

private:

    int _windowWidth;                 /**< Szerokość głównego okna aplikacji w pikselach, obliczana na podstawie geometrii ekranu. */
    int _windowHeight;                /**< Wysokość głównego okna aplikacji w pikselach, obliczana na podstawie geometrii ekranu. */
    ControllerManager *_controller;   /**< Menedżer odpowiedzialny za obsługę, połączenie i automatyczny powrót portu kontrolera UART. */
    FlightMathModel *_flightModel;     /**< Model matematyczny wyznaczający i stabilizujący położenie samolotu na podstawie danych. */

    /**
     * @brief Funkcja deklarująca główne składowe okna głównego
     * Odpowiada ułożenie elementów, utworzenie górnego i dolnego paska
     * oraz przestrzeni symulacyjnej.
     */
    void createLayout();

    /**
     * @brief Odświeża tytuł okna przy użyciu aktualnego translatora.
     */
    void reTranslate();


};

#endif // MAINWINDOW_H
